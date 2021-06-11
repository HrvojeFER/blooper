#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

DevicesMenuWindow::DevicesMenuWindow(
    AbstractCoreContext& context,
    State                state,
    Options              options)
    : CoreWindowBase(
          "Devices",
          context,
          move(state)),

      options(move(options))
{
  DevicesMenuComponent::Options componentOptions{};

  auto component = new DevicesMenuComponent(
      getContext(),
      getState().getOrCreateChildWithName(
          DevicesMenuComponent::stateId,
          nullptr),
      move(componentOptions));

  component->setBounds(this->getBounds());

  setContentOwned(component, true);
}


// Window

[[maybe_unused]] void DevicesMenuWindow::closeButtonPressed()
{
  delete this;
}


[[maybe_unused]] DevicesMenuWindow* showDevicesMenu(
    AbstractCoreContext&       context,
    DevicesMenuWindow::Options options)
{
  auto window =
      new DevicesMenuWindow(
          context,
          context.getState().getOrCreateChildWithName(
              DevicesMenuWindow::stateId,
              nullptr),
          move(options));

  window->enterModalState(
      true,
      nullptr,
      true);

  return window;
}

BLOOPER_NAMESPACE_END
