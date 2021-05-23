#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

PluginsMenuWindow::PluginsMenuWindow(AbstractCoreContext& context)
    : CoreWindowBase("Plugins", context)
{
  setContentOwned(
      new PluginsMenuComponent(context),
      true);
}


void showPluginsMenu(AbstractCoreContext& context)
{
  auto window = new PluginsMenuWindow(context);

  window->enterModalState(
      true,
      nullptr,
      true);
}

BLOOPER_NAMESPACE_END
