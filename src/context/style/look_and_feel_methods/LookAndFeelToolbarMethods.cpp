#include <blooper/context/style/LookAndFeel.hpp>

BLOOPER_NAMESPACE_BEGIN

juce::Button* LookAndFeel::createToolbarMissingItemsButton(
    juce::Toolbar& toolbar)
{
  return LookAndFeel_V2::createToolbarMissingItemsButton(
      toolbar);
}


void LookAndFeel::paintToolbarBackground(
    juce::Graphics& graphics,
    int             width,
    int             height,
    juce::Toolbar&  toolbar)
{
  LookAndFeel_V4::paintToolbarBackground(
      graphics, width, height, toolbar);
}

void LookAndFeel::paintToolbarButtonBackground(
    juce::Graphics&             graphics,
    int                         width,
    int                         height,
    bool                        isMouseOver,
    bool                        isMouseDown,
    juce::ToolbarItemComponent& component)
{
  LookAndFeel_V2::paintToolbarButtonBackground(
      graphics,
      width,
      height,
      isMouseOver,
      isMouseDown,
      component);
}

void LookAndFeel::paintToolbarButtonLabel(
    juce::Graphics&             graphics,
    int                         x,
    int                         y,
    int                         width,
    int                         height,
    const juce::String&         text,
    juce::ToolbarItemComponent& component)
{
  LookAndFeel_V4::paintToolbarButtonLabel(
      graphics, x, y, width, height, text, component);
}

BLOOPER_NAMESPACE_END
