#include <blooper/context/style/LookAndFeel.hpp>

BLOOPER_NAMESPACE_BEGIN

void LookAndFeel::drawLasso(
    juce::Graphics&  graphics,
    juce::Component& component)
{
  LookAndFeel_V2::drawLasso(
      graphics, component);
}

void LookAndFeel::drawGroupComponentOutline(
    juce::Graphics&            graphics,
    int                        w,
    int                        h,
    const juce::String&        text,
    const juce::Justification& justification,
    juce::GroupComponent&      component)
{
  LookAndFeel_V2::drawGroupComponentOutline(
      graphics, w, h, text, justification, component);
}

BLOOPER_NAMESPACE_END
