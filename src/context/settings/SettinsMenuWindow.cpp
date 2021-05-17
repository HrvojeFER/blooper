#include <blooper/blooper.hpp>


BLOOPER_NAMESPACE_BEGIN

SettingsMenuWindow::SettingsMenuWindow(CoreContext& context)
    : CoreWindow("Settings", context)
{
    setContentOwned(
            new SettingsMenuComponent(engine),
            true);
}

BLOOPER_NAMESPACE_END
