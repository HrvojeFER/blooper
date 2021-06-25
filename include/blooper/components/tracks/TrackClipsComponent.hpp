#ifndef BLOOPER_TRACK_CLIPS_COMPONENT_HPP
#define BLOOPER_TRACK_CLIPS_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

struct TrackClipsOptions
{
} options;

class TrackClipsComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(DevComponent);


  explicit TrackClipsComponent(
      AbstractContext&         context,
      State                    state,
      JuceRef<class EditTrack> track,
      TrackClipsOptions        options = {});

  ~TrackClipsComponent() override;

  TrackClipsOptions options;


 private:
  JuceRef<class EditTrack> track;


  // Component

 public:
  [[maybe_unused]] void resized() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackClipsComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_TRACK_CLIPS_COMPONENT_HPP
