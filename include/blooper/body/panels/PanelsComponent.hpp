#ifndef BLOOPER_PANELS_COMPONENT_HPP
#define BLOOPER_PANELS_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class PanelsComponent :
    public ComponentBase,

    private JuceCommandTarget
{
 public:
  BLOOPER_STATE_ID(PanelsComponent);

  BLOOPER_ID(masterTrackPanelOpenId);
  BLOOPER_ID(controlSurfacePanelOpenId);
  BLOOPER_ID(browserPanelOpenId);


  [[maybe_unused]] inline constexpr static auto defaultMasterTrackPanelSize =
      100;

  [[maybe_unused]] inline constexpr static auto defaultControlSurfacePanelSize =
      100;

  [[maybe_unused]] inline constexpr static auto defaultBrowserPanelSize =
      100;


  [[maybe_unused]] inline constexpr static auto defaultMasterTrackPanelOpen =
      true;

  [[maybe_unused]] inline constexpr static auto defaultControlSurfacePanelOpen =
      false;

  [[maybe_unused]] inline constexpr static auto defaultBrowserPanelOpen =
      false;


  struct Options
  {
  } options;

  explicit PanelsComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});

  ~PanelsComponent() override;


 private:
  JuceCached<int> masterTrackPanelSize;
  JuceCached<int> controlSurfacePanelSize;
  JuceCached<int> browserPanelSize;

  JuceCached<bool> masterTrackPanelOpen;
  JuceCached<bool> controlSurfacePanelOpen;
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


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PanelsComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PANELS_COMPONENT_HPP
