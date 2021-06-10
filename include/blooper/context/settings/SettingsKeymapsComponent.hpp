#ifndef BLOOPER_SETTINGS_KEYMAPS_COMPONENT_HPP
#define BLOOPER_SETTINGS_KEYMAPS_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class SettingsKeymapsComponent : public CoreComponentBase
{
 public:
  BLOOPER_STATE_ID(SettingsDevicesComponent);


  struct Options
  {
  } options;

  explicit SettingsKeymapsComponent(
      AbstractCoreContext& context,
      State                state,
      Options              options = {});


  // Component

 public:
  [[maybe_unused]] void resized() override;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsKeymapsComponent);
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_SETTINGS_KEYMAPS_COMPONENT_HPP
