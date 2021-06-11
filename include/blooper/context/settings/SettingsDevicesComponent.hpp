#ifndef BLOOPER_SETTINGS_DEVICES_COMPONENT_HPP
#define BLOOPER_SETTINGS_DEVICES_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class SettingsDevicesComponent : public CoreComponentBase
{
 public:
  BLOOPER_STATE_ID(SettingsDevicesComponent);


  struct Options
  {
  } options;

  explicit SettingsDevicesComponent(
      AbstractCoreContext& context,
      State                state,
      Options              options = {});


 private:
  std::unique_ptr<class DevicesMenuComponent> menu;


  // Component

 public:
  [[maybe_unused]] void resized() override;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsDevicesComponent);
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_SETTINGS_DEVICES_COMPONENT_HPP
