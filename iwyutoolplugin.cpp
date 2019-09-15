#include "iwyutoolplugin.h"
#include "generaloptionpage.h"
#include "iwyutoolconstants.h"

#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/coreconstants.h>

#include <coreplugin/icontext.h>
#include <coreplugin/icore.h>
#include <extensionsystem/pluginmanager.h>

#include <QAction>
#include <QByteArray>
#include <QDebug>
#include <QListWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <QProcess>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>

namespace IwyuTool {
namespace Internal {

IwyuToolPlugin::IwyuToolPlugin()
{
    // Create your members
}

IwyuToolPlugin::~IwyuToolPlugin()
{
    // Unregister objects from the plugin manager's object pool
    // Delete members

    for (auto object : _listObjects) {
        ExtensionSystem::PluginManager::removeObject(object);
        object->deleteLater();
    }
    _listObjects.clear();
}

bool IwyuToolPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    // Register objects in the plugin manager's object pool
    // Load settings
    // Add actions to menus
    // Connect to other plugins' signals
    // In the initialize function, a plugin can be sure that the plugins it
    // depends on have initialized their members.

    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    QSettings *settings = Core::ICore::instance()->settings();
    settings->beginGroup("IWYUPlugin");
    _toolPath = settings->value("IWYUPath", "/").toString();

    auto action = new QAction(tr("IwyuTool Action"), this);
    Core::Command *cmd = Core::ActionManager::registerAction(action,
                                                             Constants::ACTION_ID,
                                                             Core::Context(
                                                                 Core::Constants::C_GLOBAL));

    cmd->setDefaultKeySequence(QKeySequence(settings->value("IWYUPKeySeq", "").toString()));
    connect(action, &QAction::triggered, this, &IwyuToolPlugin::triggerAction);

    Core::ActionContainer *menu = Core::ActionManager::createMenu(Constants::MENU_ID);
    menu->menu()->setTitle(tr(Constants::PLUGIN_NAME));
    menu->addAction(cmd);
    Core::ActionManager::actionContainer(Core::Constants::M_TOOLS)->addMenu(menu);

    settings->endGroup();

    connect(Core::EditorManager::instance(),
            &Core::EditorManager::currentEditorChanged,
            this,
            &IwyuToolPlugin::onChangeEditor);

    _listObjects.push_back(new GeneralOptionPage());

    for (auto object : _listObjects) {
        ExtensionSystem::PluginManager::addObject(object);
    }

    return true;
}

void IwyuToolPlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized function, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
}

ExtensionSystem::IPlugin::ShutdownFlag IwyuToolPlugin::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)
    return SynchronousShutdown;
}

void IwyuToolPlugin::triggerAction() {}

void IwyuToolPlugin::appendDocument(TextEditor::TextDocument *document)
{
    if (_listDocuments.indexOf(document) == -1) {
        connect(document, &TextEditor::TextDocument::changed, this, [this, document]() {
            qDebug() << document->filePath();
            this->checkDocument(document);
        });
        connect(document, &TextEditor::TextDocument::destroyed, this, [this, document]() {
            this->_listDocuments.removeOne(document);
            qDebug() << _listDocuments;
        });
        _listDocuments.append(document);
    }
}

void IwyuToolPlugin::removeDocument(TextEditor::TextDocument *document)
{
    int index = _listDocuments.indexOf(document);
    if (index == -1) {
        return;
    }
    document->disconnect(this);
    _listDocuments.remove(index);
    qDebug() << _listDocuments;
}

void IwyuToolPlugin::removeMarksByDocument(TextEditor::TextDocument *document)
{
    for (auto mark : _listMarks[document]) {
        document->removeMark(mark);
    }
    _listMarks[document].clear();
}

void IwyuToolPlugin::checkDocument(TextEditor::TextDocument *document)
{
    removeMarksByDocument(document);

    QStringList arguments;
    arguments.append(document->filePath().toString());

    for (auto &pair : _listProcess) {
        if (pair.first == document) {
            return;
        }
    }
    auto *proc = new QProcess(this);
    QPair<TextEditor::TextDocument *, QProcess *> pair(document, proc);
    _listProcess.append(pair);

    proc->start(_toolPath, arguments);

    connect(proc,
            qOverload<int, QProcess::ExitStatus>(&QProcess::finished),
            this,
            [this, document, proc](int code, QProcess::ExitStatus status) {
                // TODO: use code and status
                Q_UNUSED(code);
                Q_UNUSED(status);
                //////////////
                QByteArray buf = proc->readAllStandardOutput();
                // TODO: use standart outpot from IWYU

                buf = proc->readAllStandardError();
                if (buf.size() > 0) {
                    QRegularExpression re(
                        R"raw(-\s(?<msg>.*\s\/\/\slines\s(?<start>\d)-(?<end>\d)).*)raw");
                    if (!re.isValid()) {
                        qDebug() << "invalid regExp";
                        return;
                    }
                    re.setPatternOptions(QRegularExpression::MultilineOption);
                    QString matchText = QString::fromUtf8(buf);
                    QRegularExpressionMatchIterator i = re.globalMatch(matchText);
                    while (i.hasNext()) {
                        QRegularExpressionMatch match = i.next();

                        TextEditor::TextMark *mark
                            = new TextEditor::TextMark(document->filePath(),
                                                       match.captured("start").toInt(),
                                                       Core::Id("Warning"));
                        mark->setLineAnnotation(match.captured("msg"));
                        // TODO: use icon for marks
                        document->addMark(mark);
                        this->_listMarks[document].append(mark);
                    }
                }
                int removedIndex = -1;
                for (int i = 0; i < this->_listProcess.count(); ++i) {
                    if (this->_listProcess[i].first == document) {
                        removedIndex = i;
                    }
                }
                if (removedIndex > -1) {
                    this->_listProcess.remove(removedIndex);
                }
                //////////////
            });
}

void IwyuToolPlugin::onChangeEditor(Core::IEditor *editor)
{
    if (!editor) {
        qDebug() << "editor nullptr";
        return;
    }
    QString typeEditor = QString::fromUtf8(editor->metaObject()->className());
    if (typeEditor != "CppEditor::Internal::CppEditor") {
        qDebug() << "not support editor type";
        return;
    }
    auto *textEditor = qobject_cast<TextEditor::BaseTextEditor *>(editor);
    if (!textEditor) {
        qDebug() << "incorrect editor";
        return;
    }
    auto *editorWidget = qobject_cast<TextEditor::TextEditorWidget *>(editor->widget());
    if (!editorWidget) {
        qDebug() << "incorrect editor widget";
        return;
    }
    auto *textDocument = qobject_cast<TextEditor::TextDocument *>(textEditor->document());
    if (!textDocument) {
        qDebug() << "incorrect document";
        return;
    }
    qDebug() << "editor initialized fine";

    appendDocument(textDocument);
}

} // namespace Internal
} // namespace IwyuTool
