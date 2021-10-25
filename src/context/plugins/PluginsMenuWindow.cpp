#include <blooper/context/plugins/PluginsMenuWindow.hpp>

#include <blooper/context/plugins/PluginsMenuComponent.hpp>

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

  auto component = new PluginsMenuComponent(
      getContext(),
      getState().getOrCreateChildWithName(
          PluginsMenuComponent::stateId,
          nullptr),
      move(componentOptions));

  component->setBounds(this->getBounds());

  setContentOwned(component, true);
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
