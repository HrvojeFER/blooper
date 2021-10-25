#include <blooper/context/settings/SettingsDevicesComponent.hpp>

#include <blooper/internal/ext/component.hpp>

#include <blooper/context/devices/DevicesMenuComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

SettingsDevicesComponent::SettingsDevicesComponent(
    AbstractCoreContext& context,
    State                state,
    Options              options)
    : CoreComponentBase(
          context,
          move(state)),
      options(move(options))
{
  DevicesMenuComponent::Options menuOptions{};

  this->menu = std::make_unique<DevicesMenuComponent>(
      this->getContext(),
      this->getState().getOrCreateChildWithName(
          DevicesMenuComponent::stateId,
          nullptr),
      move(menuOptions));


  ext::addAndMakeVisible(
      *this,
      *this->menu);
}

SettingsDevicesComponent::~SettingsDevicesComponent() = default;


// Component

[[maybe_unused]] void SettingsDevicesComponent::resized()
{
  auto availableArea = this->getLocalBounds();

  this->menu->setBounds(availableArea);
}


BLOOPER_NAMESPACE_END
