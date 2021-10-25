#ifndef BLOOPER_TRACK_CLIPS_DETAIL_CONTENT_COMPONENT_HPP
#define BLOOPER_TRACK_CLIPS_DETAIL_CONTENT_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

#include <blooper/internal/utils/FlaggedAsyncUpdater.hpp>

BLOOPER_NAMESPACE_BEGIN

struct TrackClipsDetailContentComponentOptions
{
};

class TrackClipsDetailContentComponent :
    public ComponentBase,

    private util::FlaggedAsyncUpdater
{
 public:
  BLOOPER_STATE_ID(TrackClipsDetailContentComponent);


  explicit TrackClipsDetailContentComponent(
      AbstractContext&                        context,
      State                                   state,
      JuceTrackRef                            track,
      TrackClipsDetailContentComponentOptions options = {});

  ~TrackClipsDetailContentComponent() override;

  TrackClipsDetailContentComponentOptions options;


 private:
  JuceTrackRef track;


  JuceState appearanceSettings;

  JuceCached<double> clipSize;


  juce::OwnedArray<TrackClipsDetailContentComponent> subTrackComponents;


  bool clipsUpdate;

  void updateClips();


  bool subTracksUpdate;

  void updateSubTracks();


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


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(
      TrackClipsDetailContentComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_TRACK_CLIPS_DETAIL_CONTENT_COMPONENT_HPP
