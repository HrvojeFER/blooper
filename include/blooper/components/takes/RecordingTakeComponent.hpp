#ifndef BLOOPER_RECORDING_TAKE_COMPONENT_HPP
#define BLOOPER_RECORDING_TAKE_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/time.hpp>
#include <blooper/internal/abstract/takes.hpp>
#include <blooper/internal/abstract/clips.hpp>

BLOOPER_NAMESPACE_BEGIN

struct RecordingTakeComponentOptions
{
  bool shouldResizeItself = true;
  bool shouldResizeParentWhenResized = true;

  AbstractTimeProgressConverter* converter{nullptr};
};

class RecordingTakeComponent :
    public WaveAudioTakeContentComponentBase,

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

  double punchInTime;


  struct BoundsAndTime
  {
    bool                  isValid{true};
    [[maybe_unused]] bool hasLooped{false};

    juce::Rectangle<int> bounds{};
    te::EditTimeRange    time{};
  };

  [[nodiscard]] BoundsAndTime getBoundsAndTime() const;


  void drawThumbnail(
      juce::Graphics& g,
      juce::Colour    waveformColour) const;


  void updateThumbnailAndPunchTime();

  void updatePosition();


  // Component

 public:
  void resized() override;

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
