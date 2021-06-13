#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

juce::Label* LookAndFeel::createComboBoxTextBox(
    juce::ComboBox& box)
{
  return LookAndFeel_V2::createComboBoxTextBox(
      box);
}

juce::PopupMenu::Options LookAndFeel::getOptionsForComboBoxPopupMenu(
    juce::ComboBox& box,
    juce::Label&    label)
{
  return LookAndFeel_V2::getOptionsForComboBoxPopupMenu(
      box, label);
}

void LookAndFeel::drawComboBox(
    juce::Graphics& graphics,
    int             width,
    int             height,
    bool            isButtonDown,
    int             buttonX,
    int             buttonY,
    int             buttonW,
    int             buttonH,
    juce::ComboBox& box)
{
  LookAndFeel_V4::drawComboBox(
      graphics,
      width,
      height,
      isButtonDown,
      buttonX,
      buttonY,
      buttonW,
      buttonH,
      box);
}

void LookAndFeel::drawComboBoxTextWhenNothingSelected(
    juce::Graphics& graphics,
    juce::ComboBox& box,
    juce::Label&    label)
{
  LookAndFeel_V2::drawComboBoxTextWhenNothingSelected(
      graphics, box, label);
}

juce::Font LookAndFeel::getComboBoxFont(
    juce::ComboBox& box)
{
  return LookAndFeel_V4::getComboBoxFont(
      box);
}

void LookAndFeel::positionComboBoxText(
    juce::ComboBox& box,
    juce::Label&    label)
{
  LookAndFeel_V4::positionComboBoxText(
      box, label);
}

BLOOPER_NAMESPACE_END
