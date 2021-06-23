#ifndef BLOOPER_SETTINGS_MENU_COMPONENT_HPP
#define BLOOPER_SETTINGS_MENU_COMPONENT_HPP

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

class SettingsMenuComponent : public CoreComponentBase
{
 public:
  BLOOPER_STATE_ID(SettingsMenuComponent);

  BLOOPER_ID(openedTabIndexId);


  struct Options
  {
  } options;

  explicit SettingsMenuComponent(
      AbstractCoreContext& context,
      State                state,
      Options              options = {});

  ~SettingsMenuComponent() override;


  // Component

 public:
  [[maybe_unused]] void resized() override;


 private:
  JuceCached<int> openedTabIndex;

  std::unique_ptr<class SettingsAppearanceComponent> appearance;
  std::unique_ptr<class SettingsBehaviourComponent>  behaviour;
  std::unique_ptr<class SettingsKeymapsComponent>    keymaps;
  std::unique_ptr<class SettingsDevicesComponent>    devices;
  std::unique_ptr<class SettingsPluginsComponent>    plugins;
  std::unique_ptr<class SettingsProjectsComponent>   projects;

  std::unique_ptr<juce::TabbedComponent> tabs;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsMenuComponent);
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_SETTINGS_MENU_COMPONENT_HPP
