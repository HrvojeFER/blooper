#ifndef BLOOPER_DEV_DEVICES_COMPONENT_HPP
#define BLOOPER_DEV_DEVICES_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

class DevDevicesComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(DevDevicesComponent);


  struct Options
  {
  } options;

  explicit DevDevicesComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});

  ~DevDevicesComponent() override;


  // Component

 public:
  [[maybe_unused]] void resized() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DevDevicesComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_DEV_DEVICES_COMPONENT_HPP
