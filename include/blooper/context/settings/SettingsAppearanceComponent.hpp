#ifndef BLOOPER_SETTINGS_APPEARANCE_COMPONENT_HPP
#define BLOOPER_SETTINGS_APPEARANCE_COMPONENT_HPP

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

class SettingsAppearanceComponent : public CoreComponentBase
{
 public:
  BLOOPER_STATE_ID(SettingsAppearanceComponent);

  BLOOPER_ID(panelOpennessStateId);


  struct Options
  {
  } options;

  explicit SettingsAppearanceComponent(
      AbstractCoreContext& context,
      State                state,
      Options              options = {});

  ~SettingsAppearanceComponent() override;


  // Component

 public:
  [[maybe_unused]] void resized() override;


 private:
  JuceCached<JuceString> panelOpennessState;


  JuceValueTree settingsRoot;

  JuceCached<double>
      masterTrackPanelSize,
      controlSurfacePanelSize,
      browserPanelSize;

  JuceCached<double>
      trackSize;


  std::unique_ptr<juce::PropertyPanel> panel;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsAppearanceComponent);
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_SETTINGS_APPEARANCE_COMPONENT_HPP
