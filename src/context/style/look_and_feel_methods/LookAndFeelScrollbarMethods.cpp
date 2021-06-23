#include <blooper/context/style/LookAndFeel.hpp>

BLOOPER_NAMESPACE_BEGIN

void LookAndFeel::drawScrollbar(
    juce::Graphics&  graphics,
    juce::ScrollBar& bar,
    int              x,
    int              y,
    int              width,
    int              height,
    bool             isScrollbarVertical,
    int              thumbStartPosition,
    int              thumbSize,
    bool             isMouseOver,
    bool             isMouseDown)
{
  LookAndFeel_V4::drawScrollbar(
      graphics,
      bar,
      x,
      y,
      width,
      height,
      isScrollbarVertical,
      thumbStartPosition,
      thumbSize,
      isMouseOver,
      isMouseDown);
}

void LookAndFeel::drawScrollbarButton(
    juce::Graphics&  graphics,
    juce::ScrollBar& bar,
    int              width,
    int              height,
    int              buttonDirection,
    bool             isScrollbarVertical,
    bool             shouldDrawButtonAsHighlighted,
    bool             shouldDrawButtonAsDown)
{
  LookAndFeel_V2::drawScrollbarButton(
      graphics,
      bar,
      width,
      height,
      buttonDirection,
      isScrollbarVertical,
      shouldDrawButtonAsHighlighted,
      shouldDrawButtonAsDown);
}


juce::ImageEffectFilter* LookAndFeel::getScrollbarEffect()
{
  return LookAndFeel_V2::getScrollbarEffect();
}


int LookAndFeel::getMinimumScrollbarThumbSize(
    juce::ScrollBar& bar)
{
  return LookAndFeel_V2::getMinimumScrollbarThumbSize(
      bar);
}

int LookAndFeel::getScrollbarButtonSize(
    juce::ScrollBar& bar)
{
  return LookAndFeel_V2::getScrollbarButtonSize(
      bar);
}

int LookAndFeel::getDefaultScrollbarWidth()
{
  return LookAndFeel_V4::getDefaultScrollbarWidth();
}


bool LookAndFeel::areScrollbarButtonsVisible()
{
  return LookAndFeel_V3::areScrollbarButtonsVisible();
}

BLOOPER_NAMESPACE_END
