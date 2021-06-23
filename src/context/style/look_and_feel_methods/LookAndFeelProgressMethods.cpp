#include <blooper/context/style/LookAndFeel.hpp>

BLOOPER_NAMESPACE_BEGIN

void LookAndFeel::drawSpinningWaitAnimation(
    juce::Graphics&     graphics,
    const juce::Colour& colour,
    int                 x,
    int                 y,
    int                 w,
    int                 h)
{
  LookAndFeel_V2::drawSpinningWaitAnimation(
      graphics, colour, x, y, w, h);
}


void LookAndFeel::drawProgressBar(
    juce::Graphics&     graphics,
    juce::ProgressBar&  bar,
    int                 width,
    int                 height,
    double              progress,
    const juce::String& textToShow)
{
  LookAndFeel_V4::drawProgressBar(
      graphics, bar, width, height, progress, textToShow);
}

bool LookAndFeel::isProgressBarOpaque(
    juce::ProgressBar& bar)
{
  return LookAndFeel_V4::isProgressBarOpaque(
      bar);
}

BLOOPER_NAMESPACE_END
