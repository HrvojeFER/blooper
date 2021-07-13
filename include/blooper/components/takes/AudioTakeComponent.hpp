#ifndef BLOOPER_AUDIO_TAKE_COMPONENT_HPP
#define BLOOPER_AUDIO_TAKE_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

struct AudioTakeComponentOptions
{
};

class AudioTakeComponent :
    public ComponentBase
{
 public:
  BLOOPER_STATE_ID(AudioTakeComponent);


  explicit AudioTakeComponent(
      AbstractContext&         context,
      State                    state,
      AudioTakeComponentOptions options = {});

  ~AudioTakeComponent() override;

  AudioTakeComponentOptions options;


  // Component

 public:
  void resized() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioTakeComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_AUDIO_TAKE_COMPONENT_HPP
