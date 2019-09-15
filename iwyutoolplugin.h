#ifndef IWUYTOOL_H
#define IWUYTOOL_H

#include "iwyutool_global.h"

#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/editormanager/ieditor.h>
#include <coreplugin/editormanager/ieditorfactory.h>
#include <texteditor/textdocument.h>
#include <texteditor/texteditor.h>
#include <texteditor/textmark.h>

#include <extensionsystem/iplugin.h>

#include <QHash>
#include <QPair>
#include <QProcess>

namespace IwyuTool {
namespace Internal {

class IwyuToolPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "IwyuTool.json")

public:
    IwyuToolPlugin();
    ~IwyuToolPlugin() override;

    bool initialize(const QStringList &arguments, QString *errorString) override;
    void extensionsInitialized() override;
    ShutdownFlag aboutToShutdown() override;

private:
    QVector<QObject *> _listObjects;
    QVector<TextEditor::TextDocument *> _listDocuments;
    QHash<TextEditor::TextDocument *, QVector<TextEditor::TextMark *>> _listMarks;
    QVector<QPair<TextEditor::TextDocument *, QProcess *>> _listProcess;
    QString _toolPath;

    void triggerAction();
    /**
     * @brief Добавляет документ к списку отслеживаемых.
     *  При уничтожении документа сам удаяляется из списка
     * @param document
     */
    void appendDocument(TextEditor::TextDocument *document);

    /**
     * @brief Удаляет докумнт из списка отслеживаемых
     * очищает connection
     * @param document
     */
    void removeDocument(TextEditor::TextDocument *document);
    void removeMarksByDocument(TextEditor::TextDocument *document);

    void checkDocument(TextEditor::TextDocument *document);

private slots:
    void onChangeEditor(Core::IEditor *editor);
};

} // namespace Internal
} // namespace IwyuTool

#endif // IWUYTOOL_H
