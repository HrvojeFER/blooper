#include <blooper/context/style/LookAndFeel.hpp>

BLOOPER_NAMESPACE_BEGIN

juce::Button* LookAndFeel::createSliderButton(
    juce::Slider& slider,
    bool          isIncrement)
{
  return LookAndFeel_V2::createSliderButton(
      slider, isIncrement);
}

juce::Label* LookAndFeel::createSliderTextBox(
    juce::Slider& slider)
{
  return LookAndFeel_V4::createSliderTextBox(
      slider);
}


juce::ImageEffectFilter* LookAndFeel::getSliderEffect(
    juce::Slider& slider)
{
  return LookAndFeel_V2::getSliderEffect(
      slider);
}


juce::Slider::SliderLayout LookAndFeel::getSliderLayout(
    juce::Slider& slider)
{
  return LookAndFeel_V2::getSliderLayout(
      slider);
}


juce::Font LookAndFeel::getSliderPopupFont(
    juce::Slider& slider)
{
  return LookAndFeel_V2::getSliderPopupFont(
      slider);
}

int LookAndFeel::getSliderPopupPlacement(
    juce::Slider& slider)
{
  return LookAndFeel_V2::getSliderPopupPlacement(
      slider);
}


int LookAndFeel::getSliderThumbRadius(
    juce::Slider& slider)
{
  return LookAndFeel_V4::getSliderThumbRadius(
      slider);
}


void LookAndFeel::drawLinearSliderThumb(
    juce::Graphics&                 graphics,
    int                             x,
    int                             y,
    int                             width,
    int                             height,
    float                           sliderPos,
    float                           minSliderPos,
    float                           maxSliderPos,
    const juce::Slider::SliderStyle style,
    juce::Slider&                   slider)
{
  LookAndFeel_V2::drawLinearSliderThumb(
      graphics,
      x,
      y,
      width,
      height,
      sliderPos,
      minSliderPos,
      maxSliderPos,
      style,
      slider);
}

void LookAndFeel::drawLinearSliderBackground(
    juce::Graphics&                 graphics,
    int                             x,
    int                             y,
    int                             width,
    int                             height,
    float                           sliderPos,
    float                           minSliderPos,
    float                           maxSliderPos,
    const juce::Slider::SliderStyle style,
    juce::Slider&                   slider)
{
  LookAndFeel_V3::drawLinearSliderBackground(
      graphics,
      x,
      y,
      width,
      height,
      sliderPos,
      minSliderPos,
      maxSliderPos,
      style,
      slider);
}

void LookAndFeel::drawLinearSlider(
    juce::Graphics&                 graphics,
    int                             x,
    int                             y,
    int                             width,
    int                             height,
    float                           sliderPos,
    float                           minSliderPos,
    float                           maxSliderPos,
    const juce::Slider::SliderStyle style,
    juce::Slider&                   slider)
{
  LookAndFeel_V4::drawLinearSlider(
      graphics,
      x,
      y,
      width,
      height,
      sliderPos,
      minSliderPos,
      maxSliderPos,
      style,
      slider);
}

void LookAndFeel::drawRotarySlider(
    juce::Graphics& graphics,
    int             x,
    int             y,
    int             width,
    int             height,
    float           sliderPosProportional,
    float           rotaryStartAngle,
    float           rotaryEndAngle,
    juce::Slider&   slider)
{
  LookAndFeel_V4::drawRotarySlider(
      graphics,
      x,
      y,
      width,
      height,
      sliderPosProportional,
      rotaryStartAngle,
      rotaryEndAngle,
      slider);
}


void LookAndFeel::drawLevelMeter(
    juce::Graphics& graphics,
    int             i,
    int             i1,
    float           d)
{
  LookAndFeel_V4::drawLevelMeter(
      graphics, i, i1, d);
}

BLOOPER_NAMESPACE_END
