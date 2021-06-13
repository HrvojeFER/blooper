#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

void LookAndFeel::drawTreeviewPlusMinusBox(
    juce::Graphics&               graphics,
    const juce::Rectangle<float>& area,
    juce::Colour                  backgroundColour,
    bool                          isOpen,
    bool                          isMouseOver)
{
  LookAndFeel_V3::drawTreeviewPlusMinusBox(
      graphics, area, backgroundColour, isOpen, isMouseOver);
}

bool LookAndFeel::areLinesDrawnForTreeView(
    juce::TreeView& view)
{
  return LookAndFeel_V3::areLinesDrawnForTreeView(
      view);
}

int LookAndFeel::getTreeViewIndentSize(
    juce::TreeView& view)
{
  return LookAndFeel_V3::getTreeViewIndentSize(
      view);
}

BLOOPER_NAMESPACE_END
