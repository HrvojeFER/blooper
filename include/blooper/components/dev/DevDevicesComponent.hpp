#ifndef BLOOPER_DEV_DEVICES_COMPONENT_HPP
#define BLOOPER_DEV_DEVICES_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

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


  [[maybe_unused]] void resized() override;


 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DevDevicesComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_DEV_DEVICES_COMPONENT_HPP
