#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

void LookAndFeel::drawButtonBackground(
    juce::Graphics&     graphics,
    juce::Button&       button,
    const juce::Colour& backgroundColour,
    bool                shouldDrawButtonAsHighlighted,
    bool                shouldDrawButtonAsDown)
{
  LookAndFeel_V4::drawButtonBackground(
      graphics,
      button,
      backgroundColour,
      shouldDrawButtonAsHighlighted,
      shouldDrawButtonAsDown);
}


void LookAndFeel::drawButtonText(
    juce::Graphics&   graphics,
    juce::TextButton& button,
    bool              shouldDrawButtonAsHighlighted,
    bool              shouldDrawButtonAsDown)
{
  LookAndFeel_V2::drawButtonText(
      graphics,
      button,
      shouldDrawButtonAsHighlighted,
      shouldDrawButtonAsDown);
}

int LookAndFeel::getTextButtonWidthToFitText(
    juce::TextButton& button,
    int               buttonHeight)
{
  return LookAndFeel_V2::getTextButtonWidthToFitText(
      button, buttonHeight);
}

juce::Font LookAndFeel::getTextButtonFont(
    juce::TextButton& button,
    int               buttonHeight)
{
  return LookAndFeel_V4::getTextButtonFont(
      button, buttonHeight);
}


void LookAndFeel::drawDrawableButton(
    juce::Graphics&       graphics,
    juce::DrawableButton& button,
    bool                  shouldDrawButtonAsHighlighted,
    bool                  shouldDrawButtonAsDown)
{
  LookAndFeel_V2::drawDrawableButton(
      graphics,
      button,
      shouldDrawButtonAsHighlighted,
      shouldDrawButtonAsDown);
}


void LookAndFeel::drawImageButton(
    juce::Graphics&     graphics,
    juce::Image*        image,
    int                 imageX,
    int                 imageY,
    int                 imageW,
    int                 imageH,
    const juce::Colour& overlayColour,
    float               imageOpacity,
    juce::ImageButton&  button)
{
  LookAndFeel_V2::drawImageButton(
      graphics,
      image,
      imageX,
      imageY,
      imageW,
      imageH,
      overlayColour,
      imageOpacity,
      button);
}


void LookAndFeel::drawKeymapChangeButton(
    juce::Graphics&     graphics,
    int                 width,
    int                 height,
    juce::Button&       button,
    const juce::String& keyDescription)
{
  LookAndFeel_V3::drawKeymapChangeButton(
      graphics, width, height, button, keyDescription);
}


void LookAndFeel::drawToggleButton(
    juce::Graphics&     graphics,
    juce::ToggleButton& button,
    bool                shouldDrawButtonAsHighlighted,
    bool                shouldDrawButtonAsDown)
{
  LookAndFeel_V4::drawToggleButton(
      graphics,
      button,
      shouldDrawButtonAsHighlighted,
      shouldDrawButtonAsDown);
}

void LookAndFeel::drawTickBox(
    juce::Graphics&  graphics,
    juce::Component& component,
    float            x,
    float            y,
    float            w,
    float            h,
    bool             ticked,
    bool             isEnabled,
    bool             shouldDrawButtonAsHighlighted,
    bool             shouldDrawButtonAsDown)
{
  LookAndFeel_V4::drawTickBox(
      graphics,
      component,
      x,
      y,
      w,
      h,
      ticked,
      isEnabled,
      shouldDrawButtonAsHighlighted,
      shouldDrawButtonAsDown);
}

void LookAndFeel::changeToggleButtonWidthToFitText(
    juce::ToggleButton& button)
{
  LookAndFeel_V4::changeToggleButtonWidthToFitText(
      button);
}

BLOOPER_NAMESPACE_END
