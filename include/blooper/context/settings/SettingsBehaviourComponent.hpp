#ifndef BLOOPER_SETTINGS_ENGINE_COMPONENT_HPP
#define BLOOPER_SETTINGS_ENGINE_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

class SettingsBehaviourComponent : public CoreComponentBase
{
 public:
  BLOOPER_STATE_ID(SettingsBehaviourComponent);

  BLOOPER_ID(panelOpennessStateId);


  struct Options
  {
  } options;

  explicit SettingsBehaviourComponent(
      AbstractCoreContext& context,
      State                state,
      Options              options = {});

  ~SettingsBehaviourComponent();


 private:
  JuceCached<JuceString> panelOpennessState;


  JuceState settingsRoot;

  JuceCached<JuceString> rootBrowserFolder;


  std::unique_ptr<juce::PropertyPanel> panel;


  // Component

 public:
  [[maybe_unused]] void resized() override;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsBehaviourComponent);
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_SETTINGS_ENGINE_COMPONENT_HPP
