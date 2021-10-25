#ifndef BLOOPER_RECORDING_CLIP_COMPONENT_HPP
#define BLOOPER_RECORDING_CLIP_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/tracks.hpp>

BLOOPER_NAMESPACE_BEGIN

struct RecordingClipComponentOptions
{
};

class RecordingClipComponent :
    public AudioTrackContentComponentBase
{
  using base = AudioTrackContentComponentBase;


 public:
  BLOOPER_STATE_ID(RecordingClipComponent);


  explicit RecordingClipComponent(
      AbstractContext&              context,
      State                         state,
      AudioTrackRef                 track,
      RecordingClipComponentOptions options = {});

  ~RecordingClipComponent() override;

  RecordingClipComponentOptions options;


 private:
  std::unique_ptr<class RecordingTakeComponent> takeComponent;


  // Component

 public:
  void resized() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RecordingClipComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_RECORDING_CLIP_COMPONENT_HPP
