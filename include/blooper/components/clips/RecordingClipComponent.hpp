#ifndef BLOOPER_RECORDING_CLIP_COMPONENT_HPP
#define BLOOPER_RECORDING_CLIP_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

struct RecordingClipComponentOptions
{
};

class RecordingClipComponent :
    public ComponentBase
{
 public:
  BLOOPER_STATE_ID(RecordingClipComponent);


  explicit RecordingClipComponent(
      AbstractContext&              context,
      State                         state,
      RecordingClipComponentOptions options = {});

  ~RecordingClipComponent() override;

  RecordingClipComponentOptions options;


  // Component

 public:
  void resized() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RecordingClipComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_RECORDING_CLIP_COMPONENT_HPP
