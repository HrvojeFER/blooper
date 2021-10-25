#include <blooper/context/settings/SettingsMenuWindow.hpp>

#include <blooper/context/settings/SettingsMenuComponent.hpp>

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

  auto component =
      new SettingsMenuComponent(
          getContext(),
          getState().getOrCreateChildWithName(
              stateId,
              nullptr),
          move(componentOptions));

  component->setBounds(this->getBounds());

  setContentOwned(
      component,
      true);
}


// Window

[[maybe_unused]] void SettingsMenuWindow::closeButtonPressed()
{
  delete this;
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
