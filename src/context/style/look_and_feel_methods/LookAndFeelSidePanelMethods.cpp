#include <blooper/context/style/LookAndFeel.hpp>

BLOOPER_NAMESPACE_BEGIN

juce::Font LookAndFeel::getSidePanelTitleFont(
    juce::SidePanel& panel)
{
  return LookAndFeel_V2::getSidePanelTitleFont(
      panel);
}

juce::Justification LookAndFeel::getSidePanelTitleJustification(
    juce::SidePanel& panel)
{
  return LookAndFeel_V2::getSidePanelTitleJustification(
      panel);
}

juce::Path LookAndFeel::getSidePanelDismissButtonShape(
    juce::SidePanel& panel)
{
  return LookAndFeel_V2::getSidePanelDismissButtonShape(
      panel);
}


void LookAndFeel::drawStretchableLayoutResizerBar(
    juce::Graphics& graphics,
    int             i,
    int             i1,
    bool            b,
    bool            b1,
    bool            b2)
{
  LookAndFeel_V4::drawStretchableLayoutResizerBar(
      graphics, i, i1, b, b1, b2);
}

BLOOPER_NAMESPACE_END
