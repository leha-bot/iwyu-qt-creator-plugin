#ifndef GENERALOPTIONPAGE_H
#define GENERALOPTIONPAGE_H

#include "generaloptionwidget.h"
#include <coreplugin/dialogs/ioptionspage.h>

namespace IwyuTool {
namespace Internal {
class GeneralOptionPage : public Core::IOptionsPage
{
    Q_OBJECT
public:
    GeneralOptionPage(QObject *parent = 0);

    // IOptionsPage interface
public:
    QWidget *widget();
    void apply();
    void finish();

signals:
    void changeSettings();

private:
    GeneralOptionWidget *_pageWidget = nullptr;
    void saveSetting();
    void restoreSettings();
};

} // namespace Internal
} // namespace IwyuTool

#endif // GENERALOPTIONPAGE_H
