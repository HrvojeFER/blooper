#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

void LookAndFeel::drawBubble(
    juce::Graphics&               graphics,
    juce::BubbleComponent&        component,
    const juce::Point<float>&     tip,
    const juce::Rectangle<float>& body)
{
  LookAndFeel_V2::drawBubble(
      graphics, component, tip, body);
}

BLOOPER_NAMESPACE_END
