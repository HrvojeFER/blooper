#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

DevicesMenuWindow::DevicesMenuWindow(
    AbstractCoreContext& context,
    State                state,
    Options              options)
    : CoreWindowBase(
          "Devices",
          context,
          std::move(state)),

      options(std::move(options))
{
  setContentOwned(
      new DevicesMenuComponent(
          getContext(),
          getState().getOrCreateChildWithName(
              DevicesMenuComponent::stateId,
              nullptr)),
      true);
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
          std::move(options));

  window->enterModalState(
      true,
      nullptr,
      true);

  return window;
}

BLOOPER_NAMESPACE_END
