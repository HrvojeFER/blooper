#ifndef BLOOPER_WAVE_AUDIO_TAKE_COMPONENT_HPP
#define BLOOPER_WAVE_AUDIO_TAKE_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/time.hpp>
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


 private:
  std::unique_ptr<JuceComponentThumbnail> thumbnail;


  static void drawWaveform(
      JuceGraphics&     g,
      JuceThumbnail&    thumb,
      WaveAudioTakeRef& take,
      JuceTimeRange     time,

      JuceBounds bounds,
      JuceColour colour);

  static void drawChannels(
      JuceGraphics&  g,
      JuceThumbnail& thumb,
      JuceTimeRange  time,

      JuceBounds bounds,
      bool       useHighRes,
      bool       useLeft,
      bool       useRight,
      float      leftGain,
      float      rightGain);


  void updateThumbnail();


  // ValueTreeListener

 private:
  [[maybe_unused]] void valueTreePropertyChanged(
      JuceState&                 state,
      const JuceStateIdentifier& id) override;


  // Component

 public:
  void paint(JuceGraphics& g) override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveAudioTakeComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_WAVE_AUDIO_TAKE_COMPONENT_HPP
