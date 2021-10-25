#ifndef BLOOPER_MIDI_TAKE_COMPONENT_HPP
#define BLOOPER_MIDI_TAKE_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/time.hpp>
#include <blooper/internal/abstract/takes.hpp>
#include <blooper/internal/abstract/clips.hpp>

#include <blooper/components/takes/TakeComponentOptions.hpp>

BLOOPER_NAMESPACE_BEGIN

struct MidiTakeComponentOptions : TakeComponentOptions
{
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


  // ValueTreeListener

 private:
  [[maybe_unused]] void valueTreePropertyChanged(
      JuceState&                 state,
      const JuceStateIdentifier& id) override;


  // Component

 public:
  void paint(juce::Graphics& g) override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiTakeComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_MIDI_TAKE_COMPONENT_HPP
