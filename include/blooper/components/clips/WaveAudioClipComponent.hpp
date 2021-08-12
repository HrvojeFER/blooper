#ifndef BLOOPER_AUDIO_CLIP_COMPONENT_HPP
#define BLOOPER_AUDIO_CLIP_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/takes.hpp>
#include <blooper/internal/abstract/clips.hpp>

BLOOPER_NAMESPACE_BEGIN

struct WaveAudioClipComponentOptions
{
};

class WaveAudioClipComponent :
    public WaveAudioClipContentComponentBase
{
  using base = WaveAudioClipContentComponentBase;

 public:
  BLOOPER_STATE_ID(WaveAudioClipComponent);


  explicit WaveAudioClipComponent(
      AbstractContext&              context,
      State                         state,
      JuceWaveAudioClipRef          clip,
      WaveAudioClipComponentOptions options = {});

  ~WaveAudioClipComponent() override;

  WaveAudioClipComponentOptions options;


 private:
  std::unique_ptr<class WaveAudioTakeComponent> currentTakeComponent;


  void updateCurrentTake();


  // Component

 public:
  void resized() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveAudioClipComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_AUDIO_CLIP_COMPONENT_HPP
