#ifndef BLOOPER_PROJECT_CONTENT_COMPONENT_HPP
#define BLOOPER_PROJECT_CONTENT_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class ProjectContentComponent :
    public ComponentBase,

    private util::FlaggedAsyncUpdater,
    private juce::ApplicationCommandTarget
{
 public:
  BLOOPER_STATE_ID(ProjectContentComponent);

  [[maybe_unused]] inline constexpr static auto defaultTrackSize = 100;


  struct Options
  {
  } options;

  explicit ProjectContentComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});

  ~ProjectContentComponent() override;


  void resized() override;


 private:
  JuceCached<int> trackSize;

  JuceFlag updateTracks;

  juce::OwnedArray<class EditTrackComponent> trackComponents;


  bool isValidTrackIndex(int index);

  void resizeTracks();

  void buildTracks();


  // ValueTreeListener

 private:
  void valueTreePropertyChanged(
      juce::ValueTree&        tree,
      const juce::Identifier& id) override;

  void valueTreeChildAdded(
      juce::ValueTree& tree,
      juce::ValueTree& child) override;

  void valueTreeChildRemoved(
      juce::ValueTree& tree,
      juce::ValueTree& child,
      int              index) override;

  void valueTreeChildOrderChanged(
      juce::ValueTree& tree,
      int              childAIndex,
      int              childBIndex) override;


  // FlaggedAsyncUpdater

 private:
  void handleAsyncUpdate() override;


  // ApplicationCommandTarget

 private:
  ApplicationCommandTarget* getNextCommandTarget() override;

  void getAllCommands(
      juce::Array<juce::CommandID>& commands) override;

  void getCommandInfo(
      juce::CommandID               commandID,
      juce::ApplicationCommandInfo& result) override;

  bool perform(const InvocationInfo& info) override;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectContentComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PROJECT_CONTENT_COMPONENT_HPP
