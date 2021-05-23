#ifndef BLOOPER_SETTINGS_MENU_COMPONENT_HPP
#define BLOOPER_SETTINGS_MENU_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class SettingsMenuComponent : public CoreComponentBase
{
 public:
  BLOOPER_STATE_ID(SettingsMenuComponent);

  struct Options
  {
  } options;

  explicit SettingsMenuComponent(
      AbstractCoreContext& context,
      State                state,
      Options              options = {});


 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsMenuComponent);
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_SETTINGS_MENU_COMPONENT_HPP
