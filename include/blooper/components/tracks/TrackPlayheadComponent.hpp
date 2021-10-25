#ifndef BLOOPER_PLAYHEAD_COMPONENT_HPP
#define BLOOPER_PLAYHEAD_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

#include <blooper/internal/abstract/time.hpp>

BLOOPER_NAMESPACE_BEGIN

struct TrackPlayheadOptions
{
};

class TrackPlayheadComponent :
    public ComponentBase,

    public juce::Timer
{
 public:
  BLOOPER_STATE_ID(TrackPlayheadComponent);


  explicit TrackPlayheadComponent(
      AbstractContext&     context,
      State                state,
      JuceTrackRef         track,
      TrackPlayheadOptions options = {});

  ~TrackPlayheadComponent() override;


  TrackPlayheadOptions options;


 private:
  JuceTrackRef track;


  JuceColourId colourId;


  // Component

 private:
  void paint(JuceGraphics& g) override;


  // Timer

 private:
  void timerCallback() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackPlayheadComponent);
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PLAYHEAD_COMPONENT_HPP
