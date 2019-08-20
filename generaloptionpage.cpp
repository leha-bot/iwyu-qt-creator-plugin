#include "generaloptionpage.h"
#include "iwyutoolconstants.h"

#include <QWidget>

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
    return new QWidget;
}

void GeneralOptionPage::apply()
{
    // Implement the apply botton functionality
}

void GeneralOptionPage::finish()
{
    // Implement the ok botton functionality
}

} // namespace Internal
} // namespace IwyuTool
