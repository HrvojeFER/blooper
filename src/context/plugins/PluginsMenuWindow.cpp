#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

PluginsMenuWindow::PluginsMenuWindow(
    AbstractCoreContext& context,
    State                state,
    Options              options)
    : CoreWindowBase(
          "Plugins",
          context,
          move(state)),
      options(move(options))
{
  PluginsMenuComponent::Options componentOptions{};

  setContentOwned(
      new PluginsMenuComponent(
          getContext(),
          getState().getOrCreateChildWithName(
              PluginsMenuComponent::stateId,
              nullptr),
          move(componentOptions)),
      true);
}


// Window

[[maybe_unused]] void PluginsMenuWindow::closeButtonPressed()
{
  delete this;
}


[[maybe_unused]] PluginsMenuWindow* showPluginsMenu(
    AbstractCoreContext&       context,
    PluginsMenuWindow::Options options)
{
  auto window =
      new PluginsMenuWindow(
          context,
          context.getState().getOrCreateChildWithName(
              PluginsMenuWindow::stateId,
              nullptr),
          move(options));

  window->enterModalState(
      true,
      nullptr,
      true);

  return window;
}

BLOOPER_NAMESPACE_END
