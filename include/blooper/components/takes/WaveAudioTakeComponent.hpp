#ifndef BLOOPER_AUDIO_TAKE_COMPONENT_HPP
#define BLOOPER_AUDIO_TAKE_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/takes.hpp>
#include <blooper/internal/abstract/clips.hpp>

#include <blooper/components/takes/TakeComponentOptions.hpp>

BLOOPER_NAMESPACE_BEGIN

struct WaveAudioTakeComponentOptions : TakeComponentOptions
{
};

class WaveAudioTakeComponent :
    public WaveAudioTakeContentComponentBase
{
  using base = WaveAudioTakeContentComponentBase;

 public:
  BLOOPER_STATE_ID(WaveAudioTakeComponent);


  explicit WaveAudioTakeComponent(
      AbstractContext&              context,
      State                         state,
      WaveAudioTakeRef              take,
      WaveAudioTakeComponentOptions options = {});

  ~WaveAudioTakeComponent() override;

  WaveAudioTakeComponentOptions options;


  [[nodiscard]] BoundsAndTime getBoundsAndTime() const override;


 private:
  std::unique_ptr<JuceThumbnail> thumbnail;


  [[nodiscard]] inline const WaveAudioClip* getClip() const noexcept;

  [[nodiscard]] inline WaveAudioClip* getClip() noexcept;


  static void drawWaveform(
      juce::Graphics&     g,
      te::SmartThumbnail& thumb,

      juce::Rectangle<int> area,
      juce::Colour         colour,

      te::EditTimeRange       time,
      const WaveAudioTakeRef& take);

  static void drawChannels(
      juce::Graphics&     g,
      te::SmartThumbnail& thumb,

      juce::Rectangle<int> area,

      te::EditTimeRange time,
      bool              useHighRes,
      bool              useLeft,
      bool              useRight,
      float             leftGain,
      float             rightGain);

  void updateThumbnail();


  // Component

 public:
  void resized() override;

  void paint(juce::Graphics& g) override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveAudioTakeComponent)
};


inline const WaveAudioClip* WaveAudioTakeComponent::getClip() const noexcept
{
  return this->getHeldTakeRef().clip;
}

inline WaveAudioClip* WaveAudioTakeComponent::getClip() noexcept
{
  return this->getHeldTakeRef().clip;
}

BLOOPER_NAMESPACE_END

#endif // BLOOPER_AUDIO_TAKE_COMPONENT_HPP
