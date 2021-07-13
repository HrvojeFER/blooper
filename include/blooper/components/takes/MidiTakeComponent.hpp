#ifndef BLOOPER_MIDI_TAKE_COMPONENT_HPP
#define BLOOPER_MIDI_TAKE_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

struct MidiTakeComponentOptions
{
};

class MidiTakeComponent :
    public ComponentBase
{
 public:
  BLOOPER_STATE_ID(MidiTakeComponent);


  explicit MidiTakeComponent(
      AbstractContext&         context,
      State                    state,
      MidiTakeComponentOptions options = {});

  ~MidiTakeComponent() override;

  MidiTakeComponentOptions options;


  // Component

 public:
  void resized() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiTakeComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_MIDI_TAKE_COMPONENT_HPP
