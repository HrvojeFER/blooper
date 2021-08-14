#ifndef BLOOPER_RECORDING_TAKE_COMPONENT_HPP
#define BLOOPER_RECORDING_TAKE_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/time.hpp>
#include <blooper/internal/abstract/takes.hpp>
#include <blooper/internal/abstract/clips.hpp>

#include <blooper/components/takes/TakeComponentOptions.hpp>

BLOOPER_NAMESPACE_BEGIN

struct RecordingTakeComponentOptions : TakeComponentOptions
{
};

class RecordingTakeComponent :
    public virtual WaveAudioTakeContentComponentBase,

    private JuceTimer
{
  using base = WaveAudioTakeContentComponentBase;


 public:
  BLOOPER_STATE_ID(RecordingTakeComponent);


  explicit RecordingTakeComponent(
      AbstractContext&              context,
      State                         state,
      WaveAudioTakeRef              take,
      RecordingTakeComponentOptions options = {});

  ~RecordingTakeComponent() override;

  RecordingTakeComponentOptions options;


 private:
  JuceRecordingThumbnailRef thumbnail;


  static void drawWaveform(
      JuceGraphics&  g,
      JuceThumbnail& thumb,
      JuceTimeRange  time,

      JuceBounds bounds,
      JuceColour waveformColour);


  void updateThumbnail();

  void updatePosition();


  // AbstractTimePixelConverter

 public:
  [[nodiscard]] JuceTimeRange
  getAbsoluteTimeRange() const noexcept override;


  // Component

 public:
  void paint(juce::Graphics& g) override;


  // Timer

 private:
  void timerCallback() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RecordingTakeComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_RECORDING_TAKE_COMPONENT_HPP
