#ifndef BLOOPER_TRACK_CLIPS_DETAIL_WINDOW_HPP
#define BLOOPER_TRACK_CLIPS_DETAIL_WINDOW_HPP
#pragma once

#include <blooper/internal/abstract/windows.hpp>

BLOOPER_NAMESPACE_BEGIN

struct TrackClipsDetailWindowOptions
{
};

class TrackClipsDetailWindow :
    public WindowBase
{
 public:
  BLOOPER_STATE_ID(TrackClipsDetailWindow);


  explicit TrackClipsDetailWindow(
      AbstractContext&              context,
      State                         state,
      JuceTrackRef                  track,
      TrackClipsDetailWindowOptions options = {});

  ~TrackClipsDetailWindow() override;

  TrackClipsDetailWindowOptions options;


 private:
  JuceTrackRef track;

  std::unique_ptr<class TrackClipsDetailComponent> component;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackClipsDetailWindow)
};


// Show

TrackClipsDetailWindow* showTrackClipsDetail(
    AbstractContext&              context,
    JuceTrackRef                  track,
    TrackClipsDetailWindowOptions options = {});

BLOOPER_NAMESPACE_END

#endif // BLOOPER_TRACK_CLIPS_DETAIL_WINDOW_HPP
