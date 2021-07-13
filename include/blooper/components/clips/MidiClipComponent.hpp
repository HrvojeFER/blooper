#ifndef BLOOPER_MIDI_CLIP_COMPONENT_HPP
#define BLOOPER_MIDI_CLIP_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

struct MidiClipComponentOptions
{
};

class MidiClipComponent :
    public ComponentBase
{
 public:
  BLOOPER_STATE_ID(MidiClipComponent);


  explicit MidiClipComponent(
      AbstractContext&         context,
      State                    state,
      MidiClipComponentOptions options = {});

  ~MidiClipComponent() override;

  MidiClipComponentOptions options;


  // Component

 public:
  void resized() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiClipComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_MIDI_CLIP_COMPONENT_HPP
