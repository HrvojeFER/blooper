#ifndef BLOOPER_AUDIO_CLIP_COMPONENT_HPP
#define BLOOPER_AUDIO_CLIP_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

struct AudioClipComponentOptions
{
};

class WaveAudioClipComponent :
    public ComponentBase
{
 public:
  BLOOPER_STATE_ID(WaveAudioClipComponent);


  explicit WaveAudioClipComponent(
      AbstractContext&          context,
      State                     state,
      AudioClipComponentOptions options = {});

  ~WaveAudioClipComponent() override;

  AudioClipComponentOptions options;


 private:
  std::unique_ptr<te::SmartThumbnail> thumbnail;


  // Component

 public:
  void resized() override;

  void paint(juce::Graphics& g) override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveAudioClipComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_AUDIO_CLIP_COMPONENT_HPP
