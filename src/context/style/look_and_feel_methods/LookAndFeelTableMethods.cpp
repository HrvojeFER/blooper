#include <blooper/context/style/LookAndFeel.hpp>

BLOOPER_NAMESPACE_BEGIN

void LookAndFeel::drawTableHeaderBackground(
    juce::Graphics&             graphics,
    juce::TableHeaderComponent& component)
{
  LookAndFeel_V3::drawTableHeaderBackground(
      graphics, component);
}

void LookAndFeel::drawTableHeaderColumn(
    juce::Graphics&             graphics,
    juce::TableHeaderComponent& component,
    const juce::String&         columnName,
    int                         columnId,
    int                         width,
    int                         height,
    bool                        isMouseOver,
    bool                        isMouseDown,
    int                         columnFlags)
{
  LookAndFeel_V2::drawTableHeaderColumn(
      graphics,
      component,
      columnName,
      columnId,
      width,
      height,
      isMouseOver,
      isMouseDown,
      columnFlags);
}

BLOOPER_NAMESPACE_END
