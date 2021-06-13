#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

void LookAndFeel::drawMenuBarBackground(
    juce::Graphics&         graphics,
    int                     width,
    int                     height,
    bool                    isMouseOverBar,
    juce::MenuBarComponent& component)
{
  LookAndFeel_V4::drawMenuBarBackground(
      graphics, width, height, isMouseOverBar, component);
}

void LookAndFeel::drawMenuBarItem(
    juce::Graphics&         graphics,
    int                     width,
    int                     height,
    int                     itemIndex,
    const juce::String&     itemText,
    bool                    isMouseOverItem,
    bool                    isMenuOpen,
    bool                    isMouseOverBar,
    juce::MenuBarComponent& component)
{
  LookAndFeel_V4::drawMenuBarItem(
      graphics,
      width,
      height,
      itemIndex,
      itemText,
      isMouseOverItem,
      isMenuOpen,
      isMouseOverBar,
      component);
}

int LookAndFeel::getMenuBarItemWidth(
    juce::MenuBarComponent& component,
    int                     itemIndex,
    const juce::String&     itemText)
{
  return LookAndFeel_V2::getMenuBarItemWidth(
      component, itemIndex, itemText);
}

juce::Font LookAndFeel::getMenuBarFont(
    juce::MenuBarComponent& component,
    int                     itemIndex,
    const juce::String&     itemText)
{
  return LookAndFeel_V2::getMenuBarFont(
      component, itemIndex, itemText);
}

int LookAndFeel::getDefaultMenuBarHeight()
{
  return LookAndFeel_V2::getDefaultMenuBarHeight();
}

BLOOPER_NAMESPACE_END
