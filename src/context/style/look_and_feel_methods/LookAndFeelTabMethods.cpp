#include <blooper/context/style/LookAndFeel.hpp>

BLOOPER_NAMESPACE_BEGIN

void LookAndFeel::drawTabbedButtonBarBackground(
    juce::TabbedButtonBar& bar,
    juce::Graphics&        graphics)
{
  LookAndFeel_V2::drawTabbedButtonBarBackground(
      bar, graphics);
}

void LookAndFeel::drawTabAreaBehindFrontButton(
    juce::TabbedButtonBar& bar,
    juce::Graphics&        g,
    int                    w,
    int                    h)
{
  LookAndFeel_V3::drawTabAreaBehindFrontButton(
      bar, g, w, h);
}

void LookAndFeel::drawTabButton(
    juce::TabBarButton& button,
    juce::Graphics&     graphics,
    bool                isMouseOver,
    bool                isMouseDown)
{
  LookAndFeel_V3::drawTabButton(
      button, graphics, isMouseOver, isMouseDown);
}

void LookAndFeel::drawTabButtonText(
    juce::TabBarButton& button,
    juce::Graphics&     graphics,
    bool                isMouseOver,
    bool                isMouseDown)
{
  LookAndFeel_V2::drawTabButtonText(
      button, graphics, isMouseOver, isMouseDown);
}


void LookAndFeel::createTabButtonShape(
    juce::TabBarButton& button,
    juce::Path&         path,
    bool                isMouseOver,
    bool                isMouseDown)
{
  LookAndFeel_V2::createTabButtonShape(
      button, path, isMouseOver, isMouseDown);
}

void LookAndFeel::fillTabButtonShape(
    juce::TabBarButton& button,
    juce::Graphics&     graphics,
    const juce::Path&   path,
    bool                isMouseOver,
    bool                isMouseDown)
{
  LookAndFeel_V2::fillTabButtonShape(
      button, graphics, path, isMouseOver, isMouseDown);
}


int LookAndFeel::getTabButtonBestWidth(
    juce::TabBarButton& button,
    int                 tabDepth)
{
  return LookAndFeel_V2::getTabButtonBestWidth(
      button, tabDepth);
}

juce::Font LookAndFeel::getTabButtonFont(
    juce::TabBarButton& button,
    float               height)
{
  return LookAndFeel_V2::getTabButtonFont(
      button, height);
}


juce::Rectangle<int> LookAndFeel::getTabButtonExtraComponentBounds(
    const juce::TabBarButton& button,
    juce::Rectangle<int>&     textArea,
    juce::Component&          extraComp)
{
  return LookAndFeel_V2::getTabButtonExtraComponentBounds(
      button, textArea, extraComp);
}


juce::Button* LookAndFeel::createTabBarExtrasButton()
{
  return LookAndFeel_V2::createTabBarExtrasButton();
}


int LookAndFeel::getTabButtonOverlap(
    int tabDepth)
{
  return LookAndFeel_V3::getTabButtonOverlap(
      tabDepth);
}


int LookAndFeel::getTabButtonSpaceAroundImage()
{
  return LookAndFeel_V3::getTabButtonSpaceAroundImage();
}

BLOOPER_NAMESPACE_END
