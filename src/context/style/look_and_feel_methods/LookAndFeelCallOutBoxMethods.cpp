#include <blooper/context/style/LookAndFeel.hpp>

BLOOPER_NAMESPACE_BEGIN

void LookAndFeel::drawCallOutBoxBackground(
    juce::CallOutBox& box,
    juce::Graphics&   graphics,
    const juce::Path& path,
    juce::Image&      image)
{
  LookAndFeel_V4::drawCallOutBoxBackground(
      box, graphics, path, image);
}

int LookAndFeel::getCallOutBoxBorderSize(
    const juce::CallOutBox& box)
{
  return LookAndFeel_V2::getCallOutBoxBorderSize(
      box);
}

float LookAndFeel::getCallOutBoxCornerSize(
    const juce::CallOutBox& box)
{
  return LookAndFeel_V2::getCallOutBoxCornerSize(
      box);
}

BLOOPER_NAMESPACE_END
