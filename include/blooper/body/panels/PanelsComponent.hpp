#ifndef BLOOPER_PANELS_COMPONENT_HPP
#define BLOOPER_PANELS_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

class PanelsComponent :
    public ComponentBase,

    private JuceCommandTarget
{
 public:
  BLOOPER_STATE_ID(PanelsComponent);


  struct Options
  {
  } options;

  explicit PanelsComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});

  ~PanelsComponent() override;


 private:
  JuceState appearanceSettings;

  JuceCached<int> masterTrackPanelSize;
  JuceCached<int> controlSurfacePanelSize;
  JuceCached<int> browserPanelSize;

  BLOOPER_ID(masterTrackPanelOpenId);
  JuceCached<bool> masterTrackPanelOpen;

  BLOOPER_ID(controlSurfacePanelOpenId);
  JuceCached<bool> controlSurfacePanelOpen;

  BLOOPER_ID(browserPanelOpenId);
  JuceCached<bool> browserPanelOpen;


  std::unique_ptr<class ProjectPanelComponent> projectPanel;

  std::unique_ptr<class ControlSurfacePanelComponent> controlSurfacePanel;
  std::unique_ptr<class MasterPanelComponent>         masterTrackPanel;
  std::unique_ptr<class BrowserPanelComponent>        browserPanel;


  // Component

 public:
  void resized() override;


  // ValueTreeListener

 private:
  [[maybe_unused]] void valueTreePropertyChanged(
      juce::ValueTree&        tree,
      const juce::Identifier& id) override;


  // CommandTarget

 private:
  ApplicationCommandTarget* getNextCommandTarget() override;

  void getAllCommands(juce::Array<JuceCommandId>& commands) override;

  void getCommandInfo(JuceCommandId id, JuceCommandInfo& info) override;

  bool perform(const JuceCommand& command) override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PanelsComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PANELS_COMPONENT_HPP
