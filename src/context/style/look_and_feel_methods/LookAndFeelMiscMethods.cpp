#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

std::unique_ptr<juce::LowLevelGraphicsContext>
LookAndFeel::createGraphicsContext(
    const juce::Image&              imageToRenderOn,
    juce::Point<int>                origin,
    const juce::RectangleList<int>& initialClip)
{
  return juce::LookAndFeel::createGraphicsContext(
      imageToRenderOn, origin, initialClip);
}


juce::Typeface::Ptr LookAndFeel::getTypefaceForFont(
    const juce::Font& font)
{
  return juce::LookAndFeel::getTypefaceForFont(
      font);
}


juce::MouseCursor LookAndFeel::getMouseCursorFor(
    juce::Component& component)
{
  return juce::LookAndFeel::getMouseCursorFor(
      component);
}


juce::Path LookAndFeel::getTickShape(
    float height)
{
  return LookAndFeel_V4::getTickShape(
      height);
}

juce::Path LookAndFeel::getCrossShape(
    float height)
{
  return LookAndFeel_V4::getCrossShape(
      height);
}


juce::DropShadower* LookAndFeel::createDropShadowerForComponent(
    juce::Component* component)
{
  return LookAndFeel_V2::createDropShadowerForComponent(
      component);
}

BLOOPER_NAMESPACE_END
