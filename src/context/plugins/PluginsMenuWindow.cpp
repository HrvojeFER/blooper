#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

PluginsMenuWindow::PluginsMenuWindow(
    AbstractCoreContext& context,
    State                state,
    Options              options)
    : CoreWindowBase(
          "Plugins",
          context,
          std::move(state)),
      options(std::move(options))
{
  PluginsMenuComponent::Options componentOptions{};

  setContentOwned(
      new PluginsMenuComponent(
          getContext(),
          getState().getOrCreateChildWithName(
              PluginsMenuComponent::stateId,
              nullptr),
          std::move(componentOptions)),
      true);
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
          std::move(options));

  window->enterModalState(
      true,
      nullptr,
      true);

  return window;
}

BLOOPER_NAMESPACE_END
