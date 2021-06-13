#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

void LookAndFeel::drawLabel(
    juce::Graphics& graphics,
    juce::Label&    label)
{
  LookAndFeel_V2::drawLabel(
      graphics, label);
}

juce::Font LookAndFeel::getLabelFont(
    juce::Label& label)
{
  return LookAndFeel_V2::getLabelFont(
      label);
}

juce::BorderSize<int> LookAndFeel::getLabelBorderSize(
    juce::Label& label)
{
  return LookAndFeel_V2::getLabelBorderSize(
      label);
}


void LookAndFeel::fillTextEditorBackground(
    juce::Graphics&   graphics,
    int               width,
    int               height,
    juce::TextEditor& editor)
{
  LookAndFeel_V4::fillTextEditorBackground(
      graphics, width, height, editor);
}

void LookAndFeel::drawTextEditorOutline(
    juce::Graphics&   graphics,
    int               width,
    int               height,
    juce::TextEditor& editor)
{
  LookAndFeel_V4::drawTextEditorOutline(
      graphics, width, height, editor);
}


juce::CaretComponent* LookAndFeel::createCaretComponent(
    juce::Component* keyFocusOwner)
{
  return LookAndFeel_V2::createCaretComponent(
      keyFocusOwner);
}

BLOOPER_NAMESPACE_END
