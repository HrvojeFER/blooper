#ifndef BLOOPER_DEVICES_MENU_COMPONENT_HPP
#define BLOOPER_DEVICES_MENU_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

class DevicesMenuComponent : public CoreComponentBase
{
 public:
  BLOOPER_STATE_ID(DevicesMenuComponent);


  struct Options
  {
  } options;

  explicit DevicesMenuComponent(
      AbstractCoreContext& context,
      State                state,
      Options              options = {});


  void resized() override;


 private:
  std::unique_ptr<juce::AudioDeviceSelectorComponent> selector;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DevicesMenuComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_DEVICES_MENU_COMPONENT_HPP
