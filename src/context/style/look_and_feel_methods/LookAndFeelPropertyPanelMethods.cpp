#include <blooper/context/style/LookAndFeel.hpp>

BLOOPER_NAMESPACE_BEGIN

int LookAndFeel::getPropertyPanelSectionHeaderHeight(
    const juce::String& sectionTitle)
{
  return LookAndFeel_V2::getPropertyPanelSectionHeaderHeight(
      sectionTitle);
}

void LookAndFeel::drawPropertyPanelSectionHeader(
    juce::Graphics&     graphics,
    const juce::String& name,
    bool                isOpen,
    int                 width,
    int                 height)
{
  LookAndFeel_V4::drawPropertyPanelSectionHeader(
      graphics, name, isOpen, width, height);
}


juce::Rectangle<int> LookAndFeel::getPropertyComponentContentPosition(
    juce::PropertyComponent& component)
{
  return LookAndFeel_V4::getPropertyComponentContentPosition(
      component);
}

void LookAndFeel::drawPropertyComponentBackground(
    juce::Graphics&          graphics,
    int                      width,
    int                      height,
    juce::PropertyComponent& component)
{
  LookAndFeel_V4::drawPropertyComponentBackground(
      graphics, width, height, component);
}

void LookAndFeel::drawPropertyComponentLabel(
    juce::Graphics&          graphics,
    int                      width,
    int                      height,
    juce::PropertyComponent& component)
{
  LookAndFeel_V4::drawPropertyComponentLabel(
      graphics, width, height, component);
}


void LookAndFeel::drawConcertinaPanelHeader(
    juce::Graphics&             graphics,
    const juce::Rectangle<int>& area,
    bool                        isMouseOver,
    bool                        isMouseDown,
    juce::ConcertinaPanel&      panel,
    juce::Component&            component)
{
  LookAndFeel_V4::drawConcertinaPanelHeader(
      graphics, area, isMouseOver, isMouseDown, panel, component);
}

BLOOPER_NAMESPACE_END
