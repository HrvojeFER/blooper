#ifndef BLOOPER_TRACK_CLIPS_DETAIL_COMPONENT_HPP
#define BLOOPER_TRACK_CLIPS_DETAIL_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

struct TrackClipsDetailComponentOptions
{
};

class TrackClipsDetailComponent :
    public ComponentBase
{
 public:
  BLOOPER_STATE_ID(TrackClipsDetailComponent);


  explicit TrackClipsDetailComponent(
      AbstractContext&                 context,
      State                            state,
      JuceTrackRef                     track,
      TrackClipsDetailComponentOptions options = {});

  ~TrackClipsDetailComponent() override;

  TrackClipsDetailComponentOptions options;


 private:
  JuceTrackRef track;


  BLOOPER_ID(viewportScrollRangeStartId);
  JuceCached<double> viewportScrollRangeStart;

  BLOOPER_ID(viewportScrollRangeEndId);
  JuceCached<double> viewportScrollRangeEnd;


  std::unique_ptr<class TrackClipsDetailContentComponent> content;

  std::unique_ptr<juce::Viewport> viewport;


  // Component

 public:
  void resized() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackClipsDetailComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_TRACK_CLIPS_DETAIL_COMPONENT_HPP
