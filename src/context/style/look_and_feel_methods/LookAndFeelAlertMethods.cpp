#include <blooper/context/style/LookAndFeel.hpp>

BLOOPER_NAMESPACE_BEGIN

juce::AlertWindow* LookAndFeel::createAlertWindow(
    const juce::String&              title,
    const juce::String&              message,
    const juce::String&              button1,
    const juce::String&              button2,
    const juce::String&              button3,
    juce::AlertWindow::AlertIconType iconType,
    int                              numButtons,
    juce::Component*                 associatedComponent)
{
  return LookAndFeel_V4::createAlertWindow(
      title,
      message,
      button1,
      button2,
      button3,
      iconType,
      numButtons,
      associatedComponent);
}

void LookAndFeel::drawAlertBox(
    juce::Graphics&             graphics,
    juce::AlertWindow&          window,
    const juce::Rectangle<int>& textArea,
    juce::TextLayout&           layout)
{
  LookAndFeel_V4::drawAlertBox(
      graphics, window, textArea, layout);
}

int LookAndFeel::getAlertBoxWindowFlags()
{
  return LookAndFeel_V2::getAlertBoxWindowFlags();
}

juce::Array<int> LookAndFeel::getWidthsForTextButtons(
    juce::AlertWindow&                    window,
    const juce::Array<juce::TextButton*>& array)
{
  return LookAndFeel_V2::getWidthsForTextButtons(
      window, array);
}

int LookAndFeel::getAlertWindowButtonHeight()
{
  return LookAndFeel_V4::getAlertWindowButtonHeight();
}

juce::Font LookAndFeel::getAlertWindowTitleFont()
{
  return LookAndFeel_V4::getAlertWindowTitleFont();
}

juce::Font LookAndFeel::getAlertWindowMessageFont()
{
  return LookAndFeel_V4::getAlertWindowMessageFont();
}

juce::Font LookAndFeel::getAlertWindowFont()
{
  return LookAndFeel_V4::getAlertWindowFont();
}

void LookAndFeel::playAlertSound()
{
  juce::LookAndFeel::playAlertSound();
}

BLOOPER_NAMESPACE_END
