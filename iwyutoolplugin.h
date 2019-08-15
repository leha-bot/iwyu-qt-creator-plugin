#ifndef IWUYTOOL_H
#define IWUYTOOL_H

#include "iwyutool_global.h"

#include <extensionsystem/iplugin.h>

namespace IwyuTool {
namespace Internal {

class IwyuToolPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "IwyuTool.json")

public:
    IwyuToolPlugin();
    ~IwyuToolPlugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();

private:
    void triggerAction();
};

} // namespace Internal
} // namespace IwyuTool

#endif // IWUYTOOL_H
