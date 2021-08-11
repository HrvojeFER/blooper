#ifndef BLOOPER_MIDI_TAKE_COMPONENT_HPP
#define BLOOPER_MIDI_TAKE_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/time.hpp>
#include <blooper/internal/abstract/takes.hpp>
#include <blooper/internal/abstract/clips.hpp>

BLOOPER_NAMESPACE_BEGIN

struct MidiTakeComponentOptions
{
  AbstractTimeProgressConverter* converter{nullptr};
};

class MidiTakeComponent :
    public MidiTakeContentComponentBase
{
  using base = MidiTakeContentComponentBase;

 public:
  BLOOPER_STATE_ID(MidiTakeComponent);


  explicit MidiTakeComponent(
      AbstractContext&         context,
      State                    state,
      MidiTakeRef              take,
      MidiTakeComponentOptions options = {});

  ~MidiTakeComponent() override;

  MidiTakeComponentOptions options;


 private:
  [[nodiscard]] inline const MidiClip* getClip() const noexcept;

  [[nodiscard]] inline MidiClip* getClip() noexcept;


  // Component

 public:
  void resized() override;

  void paint(juce::Graphics& g) override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiTakeComponent)
};


inline const MidiClip* MidiTakeComponent::getClip() const noexcept
{
  return this->getTakeRef().clip;
}

inline MidiClip* MidiTakeComponent::getClip() noexcept
{
  return this->getTakeRef().clip;
}

BLOOPER_NAMESPACE_END

#endif // BLOOPER_MIDI_TAKE_COMPONENT_HPP
