#ifndef BLOOPER_RECORDING_TAKE_COMPONENT_HPP
#define BLOOPER_RECORDING_TAKE_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

struct RecordingTakeComponentOptions
{
};

class RecordingTakeComponent :
    public ComponentBase
{
 public:
  BLOOPER_STATE_ID(RecordingTakeComponent);


  explicit RecordingTakeComponent(
      AbstractContext&              context,
      State                         state,
      RecordingTakeComponentOptions options = {});

  ~RecordingTakeComponent() override;

  RecordingTakeComponentOptions options;


  // Component

 public:
  void resized() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RecordingTakeComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_RECORDING_TAKE_COMPONENT_HPP
