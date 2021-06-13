#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

void LookAndFeel::drawTooltip(
    juce::Graphics&     graphics,
    const juce::String& text,
    int                 width,
    int                 height)
{
  LookAndFeel_V4::drawTooltip(
      graphics, text, width, height);
}


int LookAndFeel::getTooltipSize(
    const juce::String& string,
    int&                i,
    int&                i1)
{
  return juce::TooltipWindow::LookAndFeelMethods::getTooltipSize(
      string,
      i,
      i1);
}

juce::Rectangle<int> LookAndFeel::getTooltipBounds(
    const juce::String&  tipText,
    juce::Point<int>     screenPos,
    juce::Rectangle<int> parentArea)
{
  return LookAndFeel_V2::getTooltipBounds(
      tipText, screenPos, parentArea);
}

BLOOPER_NAMESPACE_END
