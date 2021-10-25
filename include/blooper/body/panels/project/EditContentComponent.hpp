#ifndef BLOOPER_EDIT_CONTENT_COMPONENT_HPP
#define BLOOPER_EDIT_CONTENT_COMPONENT_HPP

#include <blooper/internal/abstract/components.hpp>

#include <blooper/internal/utils/FlaggedAsyncUpdater.hpp>

BLOOPER_NAMESPACE_BEGIN

struct EditContentOptions
{
};

class EditContentComponent :
    public ComponentBase,

    private util::FlaggedAsyncUpdater,
    private juce::ApplicationCommandTarget
{
 public:
  BLOOPER_STATE_ID(EditContentComponent);


  explicit EditContentComponent(
      AbstractContext&   context,
      State              state,
      JuceEditRef        edit,
      EditContentOptions options = {});

  ~EditContentComponent() override;

  EditContentOptions options;


 private:
  JuceEditRef edit;


  JuceState appearanceSettings;

  JuceCached<int> trackSize;


  juce::OwnedArray<class TrackComponent> trackComponents;


  JuceFlag trackUpdate;

  void updateTracks();


  bool isValidTrackIndex(int index);

  void resizeTracks();


  // Component

 public:
  void resized() override;


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


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EditContentComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_EDIT_CONTENT_COMPONENT_HPP
