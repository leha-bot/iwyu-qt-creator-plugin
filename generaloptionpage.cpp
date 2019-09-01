#include "generaloptionpage.h"
#include "generaloptionwidget.h"
#include "iwyutoolconstants.h"

#include <coreplugin/icore.h>

#include <QSettings>

namespace IwyuTool {
namespace Internal {
GeneralOptionPage::GeneralOptionPage(QObject *parent)
    : Core::IOptionsPage(parent)
{
    setId(Core::Id("GeneralOptionsPageID"));
    setDisplayName(tr(Constants::PLUGIN_NAME));

    setCategory(Constants::OPTIONSPAGE_CATEGORY);
    setDisplayCategory(QLatin1String(Constants::OPTIONSPAGE_CATEGORY_TR_CATEGORY));
}

QWidget *GeneralOptionPage::widget()
{
    _pageWidget = new GeneralOptionWidget;
    restoreSettings();
    return _pageWidget;
}

void GeneralOptionPage::apply()
{
    // Implement the apply botton functionality
    saveSetting();
}

void GeneralOptionPage::finish()
{
    // Implement the ok botton functionality
}

void GeneralOptionPage::saveSetting()
{
    if (_pageWidget) {
        QSettings *settings = Core::ICore::instance()->settings();
        settings->beginGroup("IWYUPlugin");
        settings->setValue("IWYUPath", _pageWidget->getPath());
        settings->setValue("IWYUPKeySeq", _pageWidget->getKeySeq());
        settings->endGroup();
        settings->sync();
    }
}

void GeneralOptionPage::restoreSettings()
{
    if (_pageWidget) {
        QSettings *settings = Core::ICore::instance()->settings();
        settings->beginGroup("IWYUPlugin");
        _pageWidget->setPath(settings->value("IWYUPath", "/").toString());
        _pageWidget->setKeySeq(settings->value("IWYUPKeySeq", "").toString());
        settings->endGroup();
    }
}

} // namespace Internal
} // namespace IwyuTool
