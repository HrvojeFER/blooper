#include <blooper/context/style/LookAndFeel.hpp>

#include <blooper/internal/abstract/id.hpp>
#include <blooper/internal/ext/component.hpp>

#include <blooper/context/style/DefaultTheme.hpp>
#include <blooper/context/style/PropertyFileTheme.hpp>

BLOOPER_NAMESPACE_BEGIN

LookAndFeel::LookAndFeel(
    AbstractCoreContext& context)
    : CoreContextualBase(context),
      StatefulBase(
          context.getState()
              .getOrCreateChildWithName(id::appearance, nullptr)
              .getOrCreateChildWithName(id::theme, nullptr)),

      colourUpdate(false),
      themeUpdate(false)
{
  this->themePath.referTo(
      this->getState(),
      id::themePath,
      nullptr,
      "");

  this->fallback = std::make_unique<DefaultTheme>();


  this->markAndUpdate(this->themeUpdate);
}


void LookAndFeel::updateTheme()
{
  if (!JuceFile::isAbsolutePath(this->themePath.get()))
  {
    this->theme = std::make_unique<DefaultTheme>();
  }
  else
  {
    this->theme = std::make_unique<PropertyFileTheme>(this->themePath);
  }

  this->markAndUpdate(colourUpdate);
}

void LookAndFeel::updateColours()
{
  // Generic

  this->setColour(
      ColourId::background,
      this->getThemeColour(
          ColourId::background,
          ColourId::background));

  this->setColour(
      ColourId::foreground,
      this->getThemeColour(
          ColourId::foreground,
          ColourId::foreground));

  this->setColour(
      ColourId::highlight,
      this->getThemeColour(
          ColourId::highlight,
          ColourId::highlight));


  this->setColour(
      ColourId::outline,
      this->getThemeColour(
          ColourId::outline,
          ColourId::outline));

  this->setColour(
      ColourId::selection,
      this->getThemeColour(
          ColourId::selection,
          ColourId::selection));


  this->setColour(
      ColourId::cursor,
      this->getThemeColour(
          ColourId::cursor,
          ColourId::cursor));


  // Colours

  this->setColour(
      ColourId::white,
      this->getThemeColour(
          ColourId::white,
          ColourId::white));

  this->setColour(
      ColourId::whiteBright,
      this->getThemeColour(
          ColourId::whiteBright,
          ColourId::whiteBright));


  this->setColour(
      ColourId::black,
      this->getThemeColour(
          ColourId::black,
          ColourId::black));

  this->setColour(
      ColourId::blackBright,
      this->getThemeColour(
          ColourId::blackBright,
          ColourId::blackBright));


  this->setColour(
      ColourId::red,
      this->getThemeColour(
          ColourId::red,
          ColourId::red));

  this->setColour(
      ColourId::redBright,
      this->getThemeColour(
          ColourId::redBright,
          ColourId::redBright));


  this->setColour(
      ColourId::green,
      this->getThemeColour(
          ColourId::green,
          ColourId::green));

  this->setColour(
      ColourId::greenBright,
      this->getThemeColour(
          ColourId::greenBright,
          ColourId::greenBright));


  this->setColour(
      ColourId::yellow,
      this->getThemeColour(
          ColourId::yellow,
          ColourId::yellow));

  this->setColour(
      ColourId::yellowBright,
      this->getThemeColour(
          ColourId::yellowBright,
          ColourId::yellowBright));


  this->setColour(
      ColourId::blue,
      this->getThemeColour(
          ColourId::blue,
          ColourId::blue));

  this->setColour(
      ColourId::blueBright,
      this->getThemeColour(
          ColourId::blueBright,
          ColourId::blueBright));


  this->setColour(
      ColourId::purple,
      this->getThemeColour(
          ColourId::purple,
          ColourId::purple));

  this->setColour(
      ColourId::purpleBright,
      this->getThemeColour(
          ColourId::purpleBright,
          ColourId::purpleBright));


  this->setColour(
      ColourId::cyan,
      this->getThemeColour(
          ColourId::cyan,
          ColourId::cyan));

  this->setColour(
      ColourId::cyanBright,
      this->getThemeColour(
          ColourId::cyanBright,
          ColourId::cyanBright));


  // TextButton

  this->setColour(
      juce::TextButton::buttonColourId,
      this->getThemeColour(
          juce::TextButton::buttonColourId,
          ColourId::foreground));

  this->setColour(
      juce::TextButton::buttonOnColourId,
      this->getThemeColour(
          juce::TextButton::buttonOnColourId,
          ColourId::highlight));

  this->setColour(
      juce::TextButton::textColourOnId,
      this->getThemeColour(
          juce::TextButton::textColourOnId,
          ColourId::whiteBright));

  this->setColour(
      juce::TextButton::textColourOffId,
      this->getThemeColour(
          juce::TextButton::textColourOffId,
          ColourId::white));


  // ToggleButton

  this->setColour(
      juce::ToggleButton::textColourId,
      this->getThemeColour(
          juce::ToggleButton::textColourId,
          ColourId::white));

  this->setColour(
      juce::ToggleButton::tickColourId,
      this->getThemeColour(
          juce::ToggleButton::tickColourId,
          ColourId::blackBright));

  this->setColour(
      juce::ToggleButton::tickDisabledColourId,
      this->getThemeColour(
              juce::ToggleButton::tickDisabledColourId,
              ColourId::blackBright)
          .withAlpha(0.5f));


  // TextEditor

  this->setColour(
      juce::TextEditor::backgroundColourId,
      this->getThemeColour(
          juce::TextEditor::backgroundColourId,
          ColourId::background));

  this->setColour(
      juce::TextEditor::textColourId,
      this->getThemeColour(
          juce::TextEditor::textColourId,
          ColourId::white));

  this->setColour(
      juce::TextEditor::highlightColourId,
      this->getThemeColour(
              juce::TextEditor::highlightColourId,
              ColourId::highlight)
          .withAlpha(0.4f));

  this->setColour(
      juce::TextEditor::highlightedTextColourId,
      this->getThemeColour(
          juce::TextEditor::highlightedTextColourId,
          ColourId::whiteBright));

  this->setColour(
      juce::TextEditor::outlineColourId,
      this->getThemeColour(
          juce::TextEditor::outlineColourId,
          ColourId::outline));

  this->setColour(
      juce::TextEditor::focusedOutlineColourId,
      this->getThemeColour(
          juce::TextEditor::focusedOutlineColourId,
          ColourId::selection));

  this->setColour(
      juce::TextEditor::shadowColourId,
      this->getThemeColour(
          juce::TextEditor::shadowColourId,
          ColourId::transparent));


  // CaretComponent

  this->setColour(
      juce::CaretComponent::caretColourId,
      this->getThemeColour(
          juce::CaretComponent::caretColourId,
          ColourId::black));


  // Label

  this->setColour(
      juce::Label::backgroundColourId,
      this->getThemeColour(
          juce::Label::backgroundColourId,
          ColourId::transparent));

  this->setColour(
      juce::Label::textColourId,
      this->getThemeColour(
          juce::Label::textColourId,
          ColourId::white));

  this->setColour(
      juce::Label::outlineColourId,
      this->getThemeColour(
          juce::Label::outlineColourId,
          ColourId::outline));

  this->setColour(
      juce::Label::textWhenEditingColourId,
      this->getThemeColour(
          juce::Label::textWhenEditingColourId,
          ColourId::whiteBright));


  // ScrollBar

  this->setColour(
      juce::ScrollBar::backgroundColourId,
      this->getThemeColour(
          juce::ScrollBar::backgroundColourId,
          ColourId::foreground));

  this->setColour(
      juce::ScrollBar::thumbColourId,
      this->getThemeColour(
          juce::ScrollBar::thumbColourId,
          ColourId::purple));

  this->setColour(
      juce::ScrollBar::trackColourId,
      this->getThemeColour(
          juce::ScrollBar::trackColourId,
          ColourId::transparent));


  // TreeView

  this->setColour(
      juce::TreeView::linesColourId,
      this->getThemeColour(
          juce::TreeView::linesColourId,
          ColourId::transparent));

  this->setColour(
      juce::TreeView::backgroundColourId,
      this->getThemeColour(
          juce::TreeView::backgroundColourId,
          ColourId::transparent));

  this->setColour(
      juce::TreeView::dragAndDropIndicatorColourId,
      this->getThemeColour(
          juce::TreeView::dragAndDropIndicatorColourId,
          ColourId::outline));

  this->setColour(
      juce::TreeView::selectedItemBackgroundColourId,
      this->getThemeColour(
          juce::TreeView::selectedItemBackgroundColourId,
          ColourId::highlight));

  this->setColour(
      juce::TreeView::oddItemsColourId,
      this->getThemeColour(
          juce::TreeView::oddItemsColourId,
          ColourId::transparent));

  this->setColour(
      juce::TreeView::evenItemsColourId,
      this->getThemeColour(
          juce::TreeView::evenItemsColourId,
          ColourId::transparent));


  // PopupMenu

  this->setColour(
      juce::PopupMenu::backgroundColourId,
      this->getThemeColour(
          juce::PopupMenu::backgroundColourId,
          ColourId::background));

  this->setColour(
      juce::PopupMenu::textColourId,
      this->getThemeColour(
          juce::PopupMenu::textColourId,
          ColourId::white));

  this->setColour(
      juce::PopupMenu::headerTextColourId,
      this->getThemeColour(
          juce::PopupMenu::headerTextColourId,
          ColourId::yellow));

  this->setColour(
      juce::PopupMenu::highlightedTextColourId,
      this->getThemeColour(
          juce::PopupMenu::highlightedTextColourId,
          ColourId::whiteBright));

  this->setColour(
      juce::PopupMenu::highlightedBackgroundColourId,
      this->getThemeColour(
          juce::PopupMenu::highlightedBackgroundColourId,
          ColourId::highlight));


  // ComboBox

  this->setColour(
      juce::ComboBox::buttonColourId,
      this->getThemeColour(
          juce::ComboBox::buttonColourId,
          ColourId::foreground));

  this->setColour(
      juce::ComboBox::textColourId,
      this->getThemeColour(
          juce::ComboBox::textColourId,
          ColourId::white));

  this->setColour(
      juce::ComboBox::backgroundColourId,
      this->getThemeColour(
          juce::ComboBox::backgroundColourId,
          ColourId::background));

  this->setColour(
      juce::ComboBox::arrowColourId,
      this->getThemeColour(
          juce::ComboBox::arrowColourId,
          ColourId::cyan));

  this->setColour(
      juce::ComboBox::outlineColourId,
      this->getThemeColour(
          juce::ComboBox::outlineColourId,
          ColourId::outline));

  this->setColour(
      juce::ComboBox::focusedOutlineColourId,
      this->getThemeColour(
          juce::ComboBox::focusedOutlineColourId,
          ColourId::selection));


  // PropertyComponent

  this->setColour(
      juce::PropertyComponent::backgroundColourId,
      this->getThemeColour(
          juce::PropertyComponent::backgroundColourId,
          ColourId::background));

  this->setColour(
      juce::PropertyComponent::labelTextColourId,
      this->getThemeColour(
          juce::PropertyComponent::labelTextColourId,
          ColourId::white));


  // TextPropertyComponent

  this->setColour(
      juce::TextPropertyComponent::backgroundColourId,
      this->getThemeColour(
          juce::TextPropertyComponent::backgroundColourId,
          ColourId::background));

  this->setColour(
      juce::TextPropertyComponent::textColourId,
      this->getThemeColour(
          juce::TextPropertyComponent::textColourId,
          ColourId::white));

  this->setColour(
      juce::TextPropertyComponent::outlineColourId,
      this->getThemeColour(
          juce::TextPropertyComponent::outlineColourId,
          ColourId::outline));


  // BooleanPropertyComponent

  this->setColour(
      juce::BooleanPropertyComponent::backgroundColourId,
      this->getThemeColour(
          juce::BooleanPropertyComponent::backgroundColourId,
          ColourId::background));

  this->setColour(
      juce::BooleanPropertyComponent::outlineColourId,
      this->getThemeColour(
          juce::BooleanPropertyComponent::outlineColourId,
          ColourId::outline));


  // ListBox

  this->setColour(
      juce::ListBox::backgroundColourId,
      this->getThemeColour(
          juce::ListBox::backgroundColourId,
          ColourId::background));

  this->setColour(
      juce::ListBox::outlineColourId,
      this->getThemeColour(
          juce::ListBox::outlineColourId,
          ColourId::outline));

  this->setColour(
      juce::ListBox::textColourId,
      this->getThemeColour(
          juce::ListBox::textColourId,
          ColourId::white));


  // Slider

  this->setColour(
      juce::Slider::backgroundColourId,
      this->getThemeColour(
          juce::Slider::backgroundColourId,
          ColourId::background));

  this->setColour(
      juce::Slider::thumbColourId,
      this->getThemeColour(
          juce::Slider::thumbColourId,
          ColourId::purple));

  this->setColour(
      juce::Slider::trackColourId,
      this->getThemeColour(
          juce::Slider::trackColourId,
          ColourId::foreground));

  this->setColour(
      juce::Slider::rotarySliderFillColourId,
      this->getThemeColour(
          juce::Slider::rotarySliderFillColourId,
          ColourId::highlight));

  this->setColour(
      juce::Slider::rotarySliderOutlineColourId,
      this->getThemeColour(
          juce::Slider::rotarySliderOutlineColourId,
          ColourId::outline));

  this->setColour(
      juce::Slider::textBoxTextColourId,
      this->getThemeColour(
          juce::Slider::textBoxTextColourId,
          ColourId::white));

  this->setColour(
      juce::Slider::textBoxBackgroundColourId,
      this->getThemeColour(
          juce::Slider::textBoxBackgroundColourId,
          ColourId::background));

  this->setColour(
      juce::Slider::textBoxHighlightColourId,
      this->getThemeColour(
          juce::Slider::textBoxHighlightColourId,
          ColourId::highlight));

  this->setColour(
      juce::Slider::textBoxOutlineColourId,
      this->getThemeColour(
          juce::Slider::textBoxOutlineColourId,
          ColourId::outline));


  // ResizableWindow

  this->setColour(
      juce::ResizableWindow::backgroundColourId,
      this->getThemeColour(
          juce::ResizableWindow::backgroundColourId,
          ColourId::background));


  // DocumentWindow

  this->setColour(
      juce::DocumentWindow::textColourId,
      this->getThemeColour(
          juce::DocumentWindow::textColourId,
          ColourId::white));


  // AlertWindow

  this->setColour(
      juce::AlertWindow::backgroundColourId,
      this->getThemeColour(
          juce::AlertWindow::backgroundColourId,
          ColourId::background));

  this->setColour(
      juce::AlertWindow::textColourId,
      this->getThemeColour(
          juce::AlertWindow::textColourId,
          ColourId::red));

  this->setColour(
      juce::AlertWindow::outlineColourId,
      this->getThemeColour(
          juce::AlertWindow::outlineColourId,
          ColourId::outline));


  // ProgressBar

  this->setColour(
      juce::ProgressBar::backgroundColourId,
      this->getThemeColour(
          juce::ProgressBar::backgroundColourId,
          ColourId::background));

  this->setColour(
      juce::ProgressBar::foregroundColourId,
      this->getThemeColour(
          juce::ProgressBar::foregroundColourId,
          ColourId::green));


  // TooltipWindow

  this->setColour(
      juce::TooltipWindow::backgroundColourId,
      this->getThemeColour(
          juce::TooltipWindow::backgroundColourId,
          ColourId::background));

  this->setColour(
      juce::TooltipWindow::textColourId,
      this->getThemeColour(
          juce::TooltipWindow::textColourId,
          ColourId::white));

  this->setColour(
      juce::TooltipWindow::outlineColourId,
      this->getThemeColour(
          juce::TooltipWindow::outlineColourId,
          ColourId::outline));


  // Tabbed Component

  this->setColour(
      juce::TabbedComponent::backgroundColourId,
      this->getThemeColour(
          juce::TabbedComponent::backgroundColourId,
          ColourId::background));

  this->setColour(
      juce::TabbedComponent::outlineColourId,
      this->getThemeColour(
          juce::TabbedComponent::outlineColourId,
          ColourId::outline));

  this->setColour(
      juce::TabbedButtonBar::tabOutlineColourId,
      this->getThemeColour(
          juce::TabbedButtonBar::tabOutlineColourId,
          ColourId::outline));

  this->setColour(
      juce::TabbedButtonBar::frontOutlineColourId,
      this->getThemeColour(
          juce::TabbedButtonBar::frontOutlineColourId,
          ColourId::selection));


  // Toolbar

  this->setColour(
      juce::Toolbar::backgroundColourId,
      this->getThemeColour(
          juce::Toolbar::backgroundColourId,
          ColourId::background));

  this->setColour(
      juce::Toolbar::separatorColourId,
      this->getThemeColour(
          juce::Toolbar::separatorColourId,
          ColourId::outline));

  this->setColour(
      juce::Toolbar::buttonMouseOverBackgroundColourId,
      this->getThemeColour(
          juce::Toolbar::buttonMouseOverBackgroundColourId,
          ColourId::foreground));

  this->setColour(
      juce::Toolbar::buttonMouseDownBackgroundColourId,
      this->getThemeColour(
          juce::Toolbar::buttonMouseDownBackgroundColourId,
          ColourId::highlight));

  this->setColour(
      juce::Toolbar::labelTextColourId,
      this->getThemeColour(
          juce::Toolbar::labelTextColourId,
          ColourId::white));

  this->setColour(
      juce::Toolbar::editingModeOutlineColourId,
      this->getThemeColour(
          juce::Toolbar::editingModeOutlineColourId,
          ColourId::outline));


  // DrawableButton

  this->setColour(
      juce::DrawableButton::textColourId,
      this->getThemeColour(
          juce::DrawableButton::textColourId,
          ColourId::white));

  this->setColour(
      juce::DrawableButton::textColourOnId,
      this->getThemeColour(
          juce::DrawableButton::textColourOnId,
          ColourId::whiteBright));

  this->setColour(
      juce::DrawableButton::backgroundColourId,
      this->getThemeColour(
          juce::DrawableButton::backgroundColourId,
          ColourId::foreground));

  this->setColour(
      juce::DrawableButton::backgroundOnColourId,
      this->getThemeColour(
          juce::DrawableButton::backgroundOnColourId,
          ColourId::highlight));


  // HyperlinkButton

  this->setColour(
      juce::HyperlinkButton::textColourId,
      this->getThemeColour(
              juce::HyperlinkButton::textColourId,
              ColourId::white)
          .interpolatedWith(
              this->getThemeColour(
                  ColourId::blue,
                  ColourId::blueBright),
              0.4f));


  // GroupComponent

  this->setColour(
      juce::GroupComponent::outlineColourId,
      this->getThemeColour(
          juce::GroupComponent::outlineColourId,
          ColourId::outline));

  this->setColour(
      juce::GroupComponent::textColourId,
      this->getThemeColour(
          juce::GroupComponent::textColourId,
          ColourId::white));


  // BubbleComponent

  this->setColour(
      juce::BubbleComponent::backgroundColourId,
      this->getThemeColour(
          juce::BubbleComponent::backgroundColourId,
          ColourId::foreground));

  this->setColour(
      juce::BubbleComponent::outlineColourId,
      this->getThemeColour(
          juce::BubbleComponent::outlineColourId,
          ColourId::outline));


  // DirectoryContentsDisplayComponent

  this->setColour(
      juce::DirectoryContentsDisplayComponent::highlightColourId,
      this->getThemeColour(
          juce::DirectoryContentsDisplayComponent::highlightColourId,
          ColourId::highlight));

  this->setColour(
      juce::DirectoryContentsDisplayComponent::textColourId,
      this->getThemeColour(
          juce::DirectoryContentsDisplayComponent::textColourId,
          ColourId::white));

  this->setColour(
      juce::DirectoryContentsDisplayComponent::highlightedTextColourId,
      this->getThemeColour(
          juce::DirectoryContentsDisplayComponent::highlightedTextColourId,
          ColourId::whiteBright));


  // LassoComponent

  /*LassoComponent::lassoFillColourId*/
  this->setColour(
      0x1000440,
      this->getThemeColour(
              0x1000440,
              ColourId::blue)
          .withAlpha(0.5f));

  /*LassoComponent::lassoOutlineColourId*/
  this->setColour(
      0x1000441,
      this->getThemeColour(
              0x1000441,
              ColourId::blueBright)
          .withAlpha(0.5f));


  // MidiKeyboardComponent

  /*MidiKeyboardComponent::whiteNoteColourId*/
  this->setColour(
      0x1005000,
      JuceColour{0xffffffff});

  /*MidiKeyboardComponent::blackNoteColourId*/
  this->setColour(
      0x1005001,
      JuceColour{0xff000000});

  /*MidiKeyboardComponent::keySeparatorLineColourId*/
  this->setColour(
      0x1005002,
      JuceColour{0x66000000});

  /*MidiKeyboardComponent::mouseOverKeyOverlayColourId*/
  this->setColour(
      0x1005003,
      JuceColour{0x80ffff00});

  /*MidiKeyboardComponent::keyDownOverlayColourId*/
  this->setColour(
      0x1005004,
      JuceColour{0xffb6b600});

  /*MidiKeyboardComponent::textLabelColourId*/
  this->setColour(
      0x1005005,
      JuceColour{0xff000000});

  /*MidiKeyboardComponent::upDownButtonBackgroundColourId*/
  this->setColour(
      0x1005006,
      JuceColour{0xffd3d3d3});

  /*MidiKeyboardComponent::upDownButtonArrowColourId*/
  this->setColour(
      0x1005007,
      JuceColour{0xff000000});

  /*MidiKeyboardComponent::shadowColourId*/
  this->setColour(
      0x1005008,
      JuceColour{0x4c000000});


  // CodeEditorComponent

  /*CodeEditorComponent::backgroundColourId*/
  this->setColour(
      0x1004500,
      this->getThemeColour(
          0x1004500,
          ColourId::background));

  /*CodeEditorComponent::highlightColourId*/
  this->setColour(
      0x1004502,
      this->getThemeColour(
          0x1004502,
          ColourId::highlight));

  /*CodeEditorComponent::defaultTextColourId*/
  this->setColour(
      0x1004503,
      this->getThemeColour(
          0x1004503,
          ColourId::white));

  /*CodeEditorComponent::lineNumberBackgroundId*/
  this->setColour(
      0x1004504,
      this->getThemeColour(
          0x1004504,
          ColourId::foreground));

  /*CodeEditorComponent::lineNumberTextId*/
  this->setColour(
      0x1004505,
      this->getThemeColour(
          0x1004505,
          ColourId::blackBright));


  // ColourSelector

  /*ColourSelector::backgroundColourId*/
  this->setColour(
      0x1007000,
      this->getThemeColour(
          0x1007000,
          ColourId::background));

  /*ColourSelector::labelTextColourId*/
  this->setColour(
      0x1007001,
      this->getThemeColour(
          0x1007001,
          ColourId::white));


  // KeyMappingEditorComponent

  /*KeyMappingEditorComponent::backgroundColourId*/
  this->setColour(
      0x100ad00,
      this->getThemeColour(
          0x100ad00,
          ColourId::background));

  /*KeyMappingEditorComponent::textColourId*/
  this->setColour(
      0x100ad01,
      this->getThemeColour(
          0x100ad01,
          ColourId::white));


  // FileSearchingPathListComponent

  this->setColour(
      juce::FileSearchPathListComponent::backgroundColourId,
      this->getThemeColour(
          juce::FileSearchPathListComponent::backgroundColourId,
          ColourId::background));


  // FileChooserDialogBox

  this->setColour(
      juce::FileChooserDialogBox::titleTextColourId,
      this->getThemeColour(
          juce::FileChooserDialogBox::titleTextColourId,
          ColourId::white));


  // SidePanel

  this->setColour(
      juce::SidePanel::backgroundColour,
      this->getThemeColour(
          juce::SidePanel::backgroundColour,
          ColourId::background));

  this->setColour(
      juce::SidePanel::titleTextColour,
      this->getThemeColour(
          juce::SidePanel::titleTextColour,
          ColourId::white));

  this->setColour(
      juce::SidePanel::shadowBaseColour,
      this->getThemeColour(
              juce::SidePanel::shadowBaseColour,
              ColourId::black)
          .darker());

  this->setColour(
      juce::SidePanel::dismissButtonNormalColour,
      this->getThemeColour(
          juce::SidePanel::dismissButtonNormalColour,
          ColourId::background));

  this->setColour(
      juce::SidePanel::dismissButtonOverColour,
      this->getThemeColour(
              juce::SidePanel::dismissButtonOverColour,
              ColourId::foreground)
          .darker());

  this->setColour(
      juce::SidePanel::dismissButtonDownColour,
      this->getThemeColour(
              juce::SidePanel::dismissButtonDownColour,
              ColourId::highlight)
          .brighter());


  // FileBrowserComponent

  this->setColour(
      juce::FileBrowserComponent::currentPathBoxBackgroundColourId,
      this->getThemeColour(
          juce::FileBrowserComponent::currentPathBoxBackgroundColourId,
          ColourId::background));

  this->setColour(
      juce::FileBrowserComponent::currentPathBoxTextColourId,
      this->getThemeColour(
          juce::FileBrowserComponent::currentPathBoxTextColourId,
          ColourId::white));

  this->setColour(
      juce::FileBrowserComponent::currentPathBoxArrowColourId,
      this->getThemeColour(
          juce::FileBrowserComponent::currentPathBoxArrowColourId,
          ColourId::cyan));

  this->setColour(
      juce::FileBrowserComponent::filenameBoxBackgroundColourId,
      this->getThemeColour(
          juce::FileBrowserComponent::filenameBoxBackgroundColourId,
          ColourId::background));

  this->setColour(
      juce::FileBrowserComponent::filenameBoxTextColourId,
      this->getThemeColour(
          juce::FileBrowserComponent::filenameBoxTextColourId,
          ColourId::white));
}


JuceColour LookAndFeel::getThemeColour(
    JuceColourId specificId,
    JuceColourId id)
{
  if (this->theme->hasColour(specificId))
    return this->theme->getColour(specificId);

  if (this->theme->hasColour(id))
    return this->theme->getColour(id);

  if (this->fallback->hasColour(specificId))
    return this->fallback->getColour(specificId);

  return this->fallback->getColour(id);
}


// ValueTreeListener

void LookAndFeel::valueTreePropertyChanged(
    juce::ValueTree&        tree,
    const juce::Identifier& id)
{
  if (tree == this->getState())
  {
    if (id == id::themePath)
    {
      this->markAndUpdate(themeUpdate);
    }
  }
}


// FlaggedAsyncUpdater

void LookAndFeel::handleAsyncUpdate()
{
  if (util::FlaggedAsyncUpdater::compareAndReset(colourUpdate))
  {
    this->updateColours();
  }

  else if (util::FlaggedAsyncUpdater::compareAndReset(themeUpdate))
  {
    this->updateTheme();
  }

  ext::visitComponents([](auto component) {
    component->sendLookAndFeelChange();
  });
}

BLOOPER_NAMESPACE_END
