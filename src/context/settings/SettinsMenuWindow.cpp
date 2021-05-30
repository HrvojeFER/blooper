#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

SettingsMenuWindow::SettingsMenuWindow(
    AbstractCoreContext& context,
    State                state,
    Options              options)
    : CoreWindowBase(
          "Settings",
          context,
          move(state)),
      options(move(options))
{
  SettingsMenuComponent::Options componentOptions{};

  setContentOwned(
      new SettingsMenuComponent(
          getContext(),
          getState().getOrCreateChildWithName(
              stateId,
              nullptr),
          move(componentOptions)),
      true);
}


[[maybe_unused]] SettingsMenuWindow* showSettingsMenu(
    AbstractCoreContext&        context,
    SettingsMenuWindow::Options options)
{
  auto window =
      new SettingsMenuWindow(
          context,
          context.getState().getOrCreateChildWithName(
              SettingsMenuWindow::stateId,
              nullptr),
          move(options));

  window->enterModalState(
      true,
      nullptr,
      true);

  return window;
}

BLOOPER_NAMESPACE_END
