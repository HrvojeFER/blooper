#include <blooper/context/devices/DevicesMenuComponent.hpp>

#include <blooper/internal/ext/component.hpp>

BLOOPER_NAMESPACE_BEGIN

DevicesMenuComponent::DevicesMenuComponent(
    AbstractCoreContext& context,
    State                state,
    Options              options)
    : CoreComponentBase(context, move(state)),
      options(move(options))
{
  auto& manager = getContext().getEngine().getDeviceManager();
  auto& deviceManager = manager.deviceManager;

  this->selector =
      std::make_unique<juce::AudioDeviceSelectorComponent>(
          deviceManager,
          0,
          512,
          1,
          512,
          false,
          false,
          true,
          true);

  ext::addAndMakeVisible(
      *this,
      *this->selector);
}


void DevicesMenuComponent::resized()
{
  auto availableArea = getLocalBounds();

  selector->setBounds(availableArea);
}

BLOOPER_NAMESPACE_END
