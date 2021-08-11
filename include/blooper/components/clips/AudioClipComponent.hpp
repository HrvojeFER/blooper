#ifndef BLOOPER_AUDIO_CLIP_COMPONENT_HPP
#define BLOOPER_AUDIO_CLIP_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

struct AudioClipComponentOptions
{
};

class AudioClipComponent :
    public ComponentBase
{
 public:
  BLOOPER_STATE_ID(AudioClipComponent);


  explicit AudioClipComponent(
      AbstractContext&          context,
      State                     state,
      AudioClipComponentOptions options = {});

  ~AudioClipComponent() override;

  AudioClipComponentOptions options;


 private:
  std::unique_ptr<te::SmartThumbnail> thumbnail;


  // Component

 public:
  void resized() override;

  void paint(juce::Graphics& g) override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioClipComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_AUDIO_CLIP_COMPONENT_HPP
