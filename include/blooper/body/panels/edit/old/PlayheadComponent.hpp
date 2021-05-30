#ifndef BLOOPER_OLD_PLAYHEAD_COMPONENT_HPP
#define BLOOPER_OLD_PLAYHEAD_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

#include "EditViewState.hpp"

BLOOPER_NAMESPACE_BEGIN

class PlayheadComponent :
    public juce::Component,
    private juce::Timer
{
 public:
  [[maybe_unused]] PlayheadComponent(te::Edit&, EditViewState&);

  void paint(juce::Graphics& g) override;
  bool hitTest(int x, int y) override;
  void mouseDrag(const juce::MouseEvent&) override;
  void mouseDown(const juce::MouseEvent&) override;
  void mouseUp(const juce::MouseEvent&) override;


 private:
  void timerCallback() override;

  te::Edit&      edit;
  EditViewState& editViewState;

  bool firstTimer = true;
  int  xPosition = 0,
      loopStart = 0,
      loopEnd = 0;
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_OLD_PLAYHEAD_COMPONENT_HPP
