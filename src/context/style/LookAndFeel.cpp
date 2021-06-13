#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

PropertyFileTheme::PropertyFileTheme(const JuceFile& originalFile)
{
  JuceXmlFile::Options options;
  options.millisecondsBeforeSaving = 2000;
  options.storageFormat = JuceXmlFile::storeAsXML;
  options.commonToAllUsers = true;

  // If not valid, just delete it and it will create a new one when needed.
  if (!JuceXmlFile(originalFile, options).isValidFile())
    originalFile.deleteFile();

  this->file = std::make_unique<JuceXmlFile>(originalFile, options);
}

PropertyFileTheme::PropertyFileTheme(const JuceString& path)
    : PropertyFileTheme(JuceFile{path})
{
}

bool PropertyFileTheme::hasColour(JuceColourId id)
{
  return this->file->containsKey(getColourName(id));
}

JuceColour PropertyFileTheme::getColour(JuceColourId id)
{
  const auto colourXml = this->file->getXmlValue(getColourName(id));
  if (!colourXml) return JuceColour{};

  const juce::uint8 red = colourXml->getIntAttribute("red");
  const juce::uint8 green = colourXml->getIntAttribute("green");
  const juce::uint8 blue = colourXml->getIntAttribute("blue");
  const juce::uint8 alpha = colourXml->getIntAttribute("alpha");

  return JuceColour{red, green, blue, alpha};
}


// DefaultTheme

bool DefaultTheme::hasColour(JuceColourId id)
{
  return id == ColourId::transparent ||


         // Generic

         id == ColourId::background ||
         id == ColourId::outline ||

         id == ColourId::cursor ||

         id == ColourId::selection ||


         // Colours

         id == ColourId::white ||
         id == ColourId::whiteBright ||

         id == ColourId::black ||
         id == ColourId::blackBright ||

         id == ColourId::red ||
         id == ColourId::redBright ||

         id == ColourId::green ||
         id == ColourId::greenBright ||

         id == ColourId::yellow ||
         id == ColourId::yellowBright ||

         id == ColourId::blue ||
         id == ColourId::blueBright ||

         id == ColourId::purple ||
         id == ColourId::purpleBright ||

         id == ColourId::cyan ||
         id == ColourId::cyanBright;
}

JuceColour DefaultTheme::getColour(JuceColourId id)
{
  switch (id)
  {
    // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::transparent:
      return JuceColour{};


      // Generic

      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::background:
      return JuceColour{0x292d3e};

      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::outline:
      return JuceColour{0xa6accd};


      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::cursor:
      return JuceColour{0xf1f1f1};


      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::selection:
      return JuceColour{0xff5370};


      // Colours

      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::white:
      return JuceColour{0xf1f1f1};

      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::whiteBright:
      return JuceColour{0xffffff};


      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::black:
      return JuceColour{0x000000};

      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::blackBright:
      return JuceColour{0x676e95};


      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::red:
      return JuceColour{0xff5370};

      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::redBright:
      return JuceColour{0xff5370};


      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::green:
      return JuceColour{0xc3e88d};

      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::greenBright:
      return JuceColour{0xc3e88d};

      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::yellow:
      return JuceColour{0xffcb6b};

      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::yellowBright:
      return JuceColour{0xffcb6b};


      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::blue:
      return JuceColour{0x82aaff};

      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::blueBright:
      return JuceColour{0x82aaff};


      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::purple:
      return JuceColour{0xc792ea};

      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::purpleBright:
      return JuceColour{0xc792ea};


      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::cyan:
      return JuceColour{0x89ddff};

      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::cyanBright:
      return JuceColour{0x89ddff};


      // NOLINTNEXTLINE(bugprone-branch-clone)
    default:
      return JuceColour{0};
  }
}


// LookAndFeel

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

// TODO: convert to switch

void LookAndFeel::updateColours()
{
  const std::unordered_map<JuceColourId, JuceColour> idToColour{

      // Generic

      {ColourId::background,
       this->getThemeColour(
           ColourId::background,
           ColourId::background)},

      {ColourId::outline,
       this->getThemeColour(
           ColourId::outline,
           ColourId::outline)},

      {ColourId::cursor,
       this->getThemeColour(
           ColourId::cursor,
           ColourId::cursor)},

      {ColourId::selection,
       this->getThemeColour(
           ColourId::selection,
           ColourId::selection)},


      // Colours

      {ColourId::black,
       this->getThemeColour(
           ColourId::black,
           ColourId::black)},

      {ColourId::blackBright,
       this->getThemeColour(
           ColourId::blackBright,
           ColourId::blackBright)},

      {ColourId::red,
       this->getThemeColour(
           ColourId::red,
           ColourId::red)},

      {ColourId::redBright,
       this->getThemeColour(
           ColourId::redBright,
           ColourId::redBright)},

      {ColourId::green,
       this->getThemeColour(
           ColourId::green,
           ColourId::green)},

      {ColourId::greenBright,
       this->getThemeColour(
           ColourId::greenBright,
           ColourId::greenBright)},

      {ColourId::yellow,
       this->getThemeColour(
           ColourId::yellow,
           ColourId::yellow)},

      {ColourId::yellowBright,
       this->getThemeColour(
           ColourId::yellowBright,
           ColourId::yellowBright)},

      {ColourId::blue,
       this->getThemeColour(
           ColourId::blue,
           ColourId::blue)},

      {ColourId::blueBright,
       this->getThemeColour(
           ColourId::blueBright,
           ColourId::blueBright)},

      {ColourId::purple,
       this->getThemeColour(
           ColourId::purple,
           ColourId::purple)},

      {ColourId::purpleBright,
       this->getThemeColour(
           ColourId::purpleBright,
           ColourId::purpleBright)},

      {ColourId::cyan,
       this->getThemeColour(
           ColourId::cyan,
           ColourId::cyan)},

      {ColourId::cyanBright,
       this->getThemeColour(
           ColourId::cyanBright,
           ColourId::cyanBright)},

      {ColourId::white,
       this->getThemeColour(
           ColourId::white,
           ColourId::white)},

      {ColourId::whiteBright,
       this->getThemeColour(
           ColourId::whiteBright,
           ColourId::whiteBright)},


      // TextButton

      {juce::TextButton::buttonColourId,
       this->getThemeColour(
           juce::TextButton::buttonColourId,
           ColourId::background)},

      {juce::TextButton::buttonOnColourId,
       this->getThemeColour(
           juce::TextButton::buttonOnColourId,
           ColourId::selection)},

      {juce::TextButton::textColourOnId,
       this->getThemeColour(
           juce::TextButton::textColourOnId,
           ColourId::whiteBright)},

      {juce::TextButton::textColourOffId,
       this->getThemeColour(
           juce::TextButton::textColourOffId,
           ColourId::white)},


      // ToggleButton

      {juce::ToggleButton::textColourId,
       this->getThemeColour(
           juce::ToggleButton::textColourId,
           ColourId::white)},

      {juce::ToggleButton::tickColourId,
       this->getThemeColour(
           juce::ToggleButton::tickColourId,
           ColourId::white)},

      {juce::ToggleButton::tickDisabledColourId,
       this->getThemeColour(
               juce::ToggleButton::tickDisabledColourId,
               ColourId::white)
           .withAlpha(0.5f)},


      // TextEditor

      {juce::TextEditor::backgroundColourId,
       this->getThemeColour(
           juce::TextEditor::backgroundColourId,
           ColourId::background)},

      {juce::TextEditor::textColourId,
       this->getThemeColour(
           juce::TextEditor::textColourId,
           ColourId::white)},

      {juce::TextEditor::highlightColourId,
       this->getThemeColour(
               juce::TextEditor::highlightColourId,
               ColourId::selection)
           .withAlpha(0.4f)},

      {juce::TextEditor::highlightedTextColourId,
       this->getThemeColour(
           juce::TextEditor::highlightedTextColourId,
           ColourId::whiteBright)},

      {juce::TextEditor::outlineColourId,
       this->getThemeColour(
           juce::TextEditor::outlineColourId,
           ColourId::outline)},

      {juce::TextEditor::focusedOutlineColourId,
       this->getThemeColour(
           juce::TextEditor::focusedOutlineColourId,
           ColourId::outline)},

      {juce::TextEditor::shadowColourId,
       this->getThemeColour(
           juce::TextEditor::shadowColourId,
           ColourId::transparent)},


      // CaretComponent

      {juce::CaretComponent::caretColourId,
       this->getThemeColour(
           juce::CaretComponent::caretColourId,
           ColourId::black)},


      // Label

      {juce::Label::backgroundColourId,
       this->getThemeColour(
           juce::Label::backgroundColourId,
           ColourId::transparent)},

      {juce::Label::textColourId,
       this->getThemeColour(
           juce::Label::textColourId,
           ColourId::white)},

      {juce::Label::outlineColourId,
       this->getThemeColour(
           juce::Label::outlineColourId,
           ColourId::transparent)},

      {juce::Label::textWhenEditingColourId,
       this->getThemeColour(
           juce::Label::textWhenEditingColourId,
           ColourId::white)},


      // ScrollBar

      {juce::ScrollBar::backgroundColourId,
       this->getThemeColour(
           juce::ScrollBar::backgroundColourId,
           ColourId::transparent)},

      {juce::ScrollBar::thumbColourId,
       this->getThemeColour(
           juce::ScrollBar::thumbColourId,
           ColourId::purple)},

      {juce::ScrollBar::trackColourId,
       this->getThemeColour(
           juce::ScrollBar::trackColourId,
           ColourId::transparent)},


      // TreeView

      {juce::TreeView::linesColourId,
       this->getThemeColour(
           juce::TreeView::linesColourId,
           ColourId::transparent)},

      {juce::TreeView::backgroundColourId,
       this->getThemeColour(
           juce::TreeView::backgroundColourId,
           ColourId::transparent)},

      {juce::TreeView::dragAndDropIndicatorColourId,
       this->getThemeColour(
           juce::TreeView::dragAndDropIndicatorColourId,
           ColourId::outline)},

      {juce::TreeView::selectedItemBackgroundColourId,
       this->getThemeColour(
           juce::TreeView::selectedItemBackgroundColourId,
           ColourId::transparent)},

      {juce::TreeView::oddItemsColourId,
       this->getThemeColour(
           juce::TreeView::oddItemsColourId,
           ColourId::transparent)},

      {juce::TreeView::evenItemsColourId,
       this->getThemeColour(
           juce::TreeView::evenItemsColourId,
           ColourId::transparent)},


      // PopupMenu

      {juce::PopupMenu::backgroundColourId,
       this->getThemeColour(
           juce::PopupMenu::backgroundColourId,
           ColourId::background)},

      {juce::PopupMenu::textColourId,
       this->getThemeColour(
           juce::PopupMenu::textColourId,
           ColourId::white)},

      {juce::PopupMenu::headerTextColourId,
       this->getThemeColour(
           juce::PopupMenu::headerTextColourId,
           ColourId::white)},

      {juce::PopupMenu::highlightedTextColourId,
       this->getThemeColour(
           juce::PopupMenu::highlightedTextColourId,
           ColourId::whiteBright)},

      {juce::PopupMenu::highlightedBackgroundColourId,
       this->getThemeColour(
           juce::PopupMenu::highlightedBackgroundColourId,
           ColourId::selection)},


      // ComboBox

      {juce::ComboBox::buttonColourId,
       this->getThemeColour(
           juce::ComboBox::buttonColourId,
           ColourId::outline)},

      {juce::ComboBox::outlineColourId,
       this->getThemeColour(
           juce::ComboBox::outlineColourId,
           ColourId::outline)},

      {juce::ComboBox::textColourId,
       this->getThemeColour(
           juce::ComboBox::textColourId,
           ColourId::white)},

      {juce::ComboBox::backgroundColourId,
       this->getThemeColour(
           juce::ComboBox::backgroundColourId,
           ColourId::background)},

      {juce::ComboBox::arrowColourId,
       this->getThemeColour(
           juce::ComboBox::arrowColourId,
           ColourId::white)},

      {juce::ComboBox::focusedOutlineColourId,
       this->getThemeColour(
           juce::ComboBox::focusedOutlineColourId,
           ColourId::outline)},


      // PropertyComponent

      {juce::PropertyComponent::backgroundColourId,
       this->getThemeColour(
           juce::PropertyComponent::backgroundColourId,
           ColourId::background)},

      {juce::PropertyComponent::labelTextColourId,
       this->getThemeColour(
           juce::PropertyComponent::labelTextColourId,
           ColourId::white)},


      // TextPropertyComponent

      {juce::TextPropertyComponent::backgroundColourId,
       this->getThemeColour(
           juce::TextPropertyComponent::backgroundColourId,
           ColourId::background)},

      {juce::TextPropertyComponent::textColourId,
       this->getThemeColour(
           juce::TextPropertyComponent::textColourId,
           ColourId::white)},

      {juce::TextPropertyComponent::outlineColourId,
       this->getThemeColour(
           juce::TextPropertyComponent::outlineColourId,
           ColourId::outline)},


      // BooleanPropertyComponent

      {juce::BooleanPropertyComponent::backgroundColourId,
       this->getThemeColour(
           juce::BooleanPropertyComponent::backgroundColourId,
           ColourId::background)},

      {juce::BooleanPropertyComponent::outlineColourId,
       this->getThemeColour(
           juce::BooleanPropertyComponent::outlineColourId,
           ColourId::outline)},


      // ListBox

      {juce::ListBox::backgroundColourId,
       this->getThemeColour(
           juce::ListBox::backgroundColourId,
           ColourId::background)},

      {juce::ListBox::outlineColourId,
       this->getThemeColour(
           juce::ListBox::outlineColourId,
           ColourId::outline)},

      {juce::ListBox::textColourId,
       this->getThemeColour(
           juce::ListBox::textColourId,
           ColourId::white)},


      // Slider

      {juce::Slider::backgroundColourId,
       this->getThemeColour(
           juce::Slider::backgroundColourId,
           ColourId::background)},

      {juce::Slider::thumbColourId,
       this->getThemeColour(
           juce::Slider::thumbColourId,
           ColourId::black)},

      {juce::Slider::trackColourId,
       this->getThemeColour(
           juce::Slider::trackColourId,
           ColourId::selection)},

      {juce::Slider::rotarySliderFillColourId,
       this->getThemeColour(
           juce::Slider::rotarySliderFillColourId,
           ColourId::selection)},

      {juce::Slider::rotarySliderOutlineColourId,
       this->getThemeColour(
           juce::Slider::rotarySliderOutlineColourId,
           ColourId::background)},

      {juce::Slider::textBoxTextColourId,
       this->getThemeColour(
           juce::Slider::textBoxTextColourId,
           ColourId::white)},

      {juce::Slider::textBoxBackgroundColourId,
       this->getThemeColour(
               juce::Slider::textBoxBackgroundColourId,
               ColourId::background)
           .withAlpha(0.0f)},

      {juce::Slider::textBoxHighlightColourId,
       this->getThemeColour(
               juce::Slider::textBoxHighlightColourId,
               ColourId::black)
           .withAlpha(0.4f)},

      {juce::Slider::textBoxOutlineColourId,
       this->getThemeColour(
           juce::Slider::textBoxOutlineColourId,
           ColourId::outline)},


      // ResizableWindow

      {juce::ResizableWindow::backgroundColourId,
       this->getThemeColour(
           juce::ResizableWindow::backgroundColourId,
           ColourId::background)},


      // DocumentWindow

      {juce::DocumentWindow::textColourId,
       this->getThemeColour(
           juce::DocumentWindow::textColourId,
           ColourId::white)},


      // AlertWindow

      {juce::AlertWindow::backgroundColourId,
       this->getThemeColour(
           juce::AlertWindow::backgroundColourId,
           ColourId::background)},

      {juce::AlertWindow::textColourId,
       this->getThemeColour(
           juce::AlertWindow::textColourId,
           ColourId::white)},

      {juce::AlertWindow::outlineColourId,
       this->getThemeColour(
           juce::AlertWindow::outlineColourId,
           ColourId::outline)},


      // ProgressBar

      {juce::ProgressBar::backgroundColourId,
       this->getThemeColour(
           juce::ProgressBar::backgroundColourId,
           ColourId::background)},

      {juce::ProgressBar::foregroundColourId,
       this->getThemeColour(
           juce::ProgressBar::foregroundColourId,
           ColourId::selection)},


      // TooltipWindow

      {juce::TooltipWindow::backgroundColourId,
       this->getThemeColour(
           juce::TooltipWindow::backgroundColourId,
           ColourId::selection)},

      {juce::TooltipWindow::textColourId,
       this->getThemeColour(
           juce::TooltipWindow::textColourId,
           ColourId::whiteBright)},

      {juce::TooltipWindow::outlineColourId,
       this->getThemeColour(
           juce::TooltipWindow::outlineColourId,
           ColourId::transparent)},


      // Tabbed Component

      {juce::TabbedComponent::backgroundColourId,
       this->getThemeColour(
           juce::TabbedComponent::backgroundColourId,
           ColourId::transparent)},

      {juce::TabbedComponent::outlineColourId,
       this->getThemeColour(
           juce::TabbedComponent::outlineColourId,
           ColourId::outline)},

      {juce::TabbedButtonBar::tabOutlineColourId,
       this->getThemeColour(
               juce::TabbedButtonBar::tabOutlineColourId,
               ColourId::outline)
           .withAlpha(0.5f)},

      {juce::TabbedButtonBar::frontOutlineColourId,
       this->getThemeColour(
           juce::TabbedButtonBar::frontOutlineColourId,
           ColourId::outline)},


      // Toolbar

      {juce::Toolbar::backgroundColourId,
       this->getThemeColour(
               juce::Toolbar::backgroundColourId,
               ColourId::background)
           .withAlpha(0.4f)},

      {juce::Toolbar::separatorColourId,
       this->getThemeColour(
           juce::Toolbar::separatorColourId,
           ColourId::outline)},

      {juce::Toolbar::buttonMouseOverBackgroundColourId,
       this->getThemeColour(
               juce::Toolbar::buttonMouseOverBackgroundColourId,
               ColourId::background)
           .contrasting(0.2f)},

      {juce::Toolbar::buttonMouseDownBackgroundColourId,
       this->getThemeColour(
               juce::Toolbar::buttonMouseDownBackgroundColourId,
               ColourId::background)
           .contrasting(0.5f)},

      {juce::Toolbar::labelTextColourId,
       this->getThemeColour(
           juce::Toolbar::labelTextColourId,
           ColourId::white)},

      {juce::Toolbar::editingModeOutlineColourId,
       this->getThemeColour(
           juce::Toolbar::editingModeOutlineColourId,
           ColourId::outline)},


      // DrawableButton

      {juce::DrawableButton::textColourId,
       this->getThemeColour(
           juce::DrawableButton::textColourId,
           ColourId::white)},

      {juce::DrawableButton::textColourOnId,
       this->getThemeColour(
           juce::DrawableButton::textColourOnId,
           ColourId::whiteBright)},

      {juce::DrawableButton::backgroundColourId,
       this->getThemeColour(
           juce::DrawableButton::backgroundColourId,
           ColourId::transparent)},

      {juce::DrawableButton::backgroundOnColourId,
       this->getThemeColour(
           juce::DrawableButton::backgroundOnColourId,
           ColourId::selection)},


      // HyperlinkButton

      {juce::HyperlinkButton::textColourId,
       this->getThemeColour(
               juce::HyperlinkButton::textColourId,
               ColourId::white)
           .interpolatedWith(
               this->getThemeColour(
                   ColourId::blue,
                   ColourId::blueBright),
               0.4f)},


      // GroupComponent

      {juce::GroupComponent::outlineColourId,
       this->getThemeColour(
           juce::GroupComponent::outlineColourId,
           ColourId::outline)},

      {juce::GroupComponent::textColourId,
       this->getThemeColour(
           juce::GroupComponent::textColourId,
           ColourId::white)},


      // BubbleComponent

      {juce::BubbleComponent::backgroundColourId,
       this->getThemeColour(
           juce::BubbleComponent::backgroundColourId,
           ColourId::background)},

      {juce::BubbleComponent::outlineColourId,
       this->getThemeColour(
           juce::BubbleComponent::outlineColourId,
           ColourId::outline)},


      // DirectoryContentsDisplayComponent

      {juce::DirectoryContentsDisplayComponent::highlightColourId,
       this->getThemeColour(
           juce::DirectoryContentsDisplayComponent::highlightColourId,
           ColourId::selection)},

      {juce::DirectoryContentsDisplayComponent::textColourId,
       this->getThemeColour(
           juce::DirectoryContentsDisplayComponent::textColourId,
           ColourId::white)},

      {juce::DirectoryContentsDisplayComponent::highlightedTextColourId,
       this->getThemeColour(
           juce::DirectoryContentsDisplayComponent::highlightedTextColourId,
           ColourId::whiteBright)},


      // LassoComponent

      /*LassoComponent::lassoFillColourId*/
      {0x1000440,
       this->getThemeColour(
           0x1000440,
           ColourId::black)},
      /*LassoComponent::lassoOutlineColourId*/
      {0x1000441,
       this->getThemeColour(
           0x1000441,
           ColourId::outline)},


      // MidiKeyboardComponent

      /*MidiKeyboardComponent::whiteNoteColourId*/
      {0x1005000,
       JuceColour{0xffffffff}},

      /*MidiKeyboardComponent::blackNoteColourId*/
      {0x1005001,
       JuceColour{0xff000000}},

      /*MidiKeyboardComponent::keySeparatorLineColourId*/
      {0x1005002,
       JuceColour{0x66000000}},

      /*MidiKeyboardComponent::mouseOverKeyOverlayColourId*/
      {0x1005003,
       JuceColour{0x80ffff00}},

      /*MidiKeyboardComponent::keyDownOverlayColourId*/
      {0x1005004,
       JuceColour{0xffb6b600}},

      /*MidiKeyboardComponent::textLabelColourId*/
      {0x1005005,
       JuceColour{0xff000000}},

      /*MidiKeyboardComponent::upDownButtonBackgroundColourId*/
      {0x1005006,
       JuceColour{0xffd3d3d3}},

      /*MidiKeyboardComponent::upDownButtonArrowColourId*/
      {0x1005007,
       JuceColour{0xff000000}},

      /*MidiKeyboardComponent::shadowColourId*/
      {0x1005008,
       JuceColour{0x4c000000}},


      // CodeEditorComponent

      /*CodeEditorComponent::backgroundColourId*/
      {0x1004500,
       this->getThemeColour(
           0x1004500,
           ColourId::background)},

      /*CodeEditorComponent::highlightColourId*/
      {0x1004502,
       this->getThemeColour(
               0x1004502,
               ColourId::black)
           .withAlpha(0.4f)},

      /*CodeEditorComponent::defaultTextColourId*/
      {0x1004503,
       this->getThemeColour(
           0x1004503,
           ColourId::white)},

      /*CodeEditorComponent::lineNumberBackgroundId*/
      {0x1004504,
       this->getThemeColour(
               0x1004504,
               ColourId::selection)
           .withAlpha(0.5f)},

      /*CodeEditorComponent::lineNumberTextId*/
      {0x1004505,
       this->getThemeColour(
           0x1004505,
           ColourId::black)},


      // ColourSelector

      /*ColourSelector::backgroundColourId*/
      {0x1007000,
       this->getThemeColour(
           0x1007000,
           ColourId::background)},

      /*ColourSelector::labelTextColourId*/
      {0x1007001,
       this->getThemeColour(
           0x1007001,
           ColourId::white)},


      // KeyMappingEditorComponent

      /*KeyMappingEditorComponent::backgroundColourId*/
      {0x100ad00,
       this->getThemeColour(
           0x100ad00,
           ColourId::background)},

      /*KeyMappingEditorComponent::textColourId*/
      {0x100ad01,
       this->getThemeColour(
           0x100ad01,
           ColourId::white)},


      // FileSearchingPathListComponent

      {juce::FileSearchPathListComponent::backgroundColourId,
       this->getThemeColour(
           juce::FileSearchPathListComponent::backgroundColourId,
           ColourId::background)},


      // FileChooserDialogBox

      {juce::FileChooserDialogBox::titleTextColourId,
       this->getThemeColour(
           juce::FileChooserDialogBox::titleTextColourId,
           ColourId::white)},


      // SidePanel

      {juce::SidePanel::backgroundColour,
       this->getThemeColour(
           juce::SidePanel::backgroundColour,
           ColourId::background)},

      {juce::SidePanel::titleTextColour,
       this->getThemeColour(
           juce::SidePanel::titleTextColour,
           ColourId::white)},

      {juce::SidePanel::shadowBaseColour,
       this->getThemeColour(
               juce::SidePanel::shadowBaseColour,
               ColourId::background)
           .darker()},

      {juce::SidePanel::dismissButtonNormalColour,
       this->getThemeColour(
           juce::SidePanel::dismissButtonNormalColour,
           ColourId::black)},

      {juce::SidePanel::dismissButtonOverColour,
       this->getThemeColour(
               juce::SidePanel::dismissButtonOverColour,
               ColourId::black)
           .darker()},

      {juce::SidePanel::dismissButtonDownColour,
       this->getThemeColour(
               juce::SidePanel::dismissButtonDownColour,
               ColourId::black)
           .brighter()},


      // FileBrowserComponent

      {juce::FileBrowserComponent::currentPathBoxBackgroundColourId,
       this->getThemeColour(
           juce::FileBrowserComponent::currentPathBoxBackgroundColourId,
           ColourId::background)},

      {juce::FileBrowserComponent::currentPathBoxTextColourId,
       this->getThemeColour(
           juce::FileBrowserComponent::currentPathBoxTextColourId,
           ColourId::white)},

      {juce::FileBrowserComponent::currentPathBoxArrowColourId,
       this->getThemeColour(
           juce::FileBrowserComponent::currentPathBoxArrowColourId,
           ColourId::white)},

      {juce::FileBrowserComponent::filenameBoxBackgroundColourId,
       this->getThemeColour(
           juce::FileBrowserComponent::filenameBoxBackgroundColourId,
           ColourId::background)},

      {juce::FileBrowserComponent::filenameBoxTextColourId,
       this->getThemeColour(
           juce::FileBrowserComponent::filenameBoxTextColourId,
           ColourId::white)}};


  for (auto [id, colour] : idToColour)
  {
    this->setColour(move(id), move(colour));
  }
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


// LookAndFeel

juce::Typeface::Ptr LookAndFeel::getTypefaceForFont(
    const juce::Font& font)
{
  return juce::LookAndFeel::getTypefaceForFont(
      font);
}

juce::MouseCursor LookAndFeel::getMouseCursorFor(
    juce::Component& component)
{
  return juce::LookAndFeel::getMouseCursorFor(
      component);
}

std::unique_ptr<juce::LowLevelGraphicsContext>
LookAndFeel::createGraphicsContext(
    const juce::Image&              imageToRenderOn,
    juce::Point<int>                origin,
    const juce::RectangleList<int>& initialClip)
{
  return juce::LookAndFeel::createGraphicsContext(
      imageToRenderOn, origin, initialClip);
}

void LookAndFeel::playAlertSound()
{
  juce::LookAndFeel::playAlertSound();
}

void LookAndFeel::drawButtonText(
    juce::Graphics&   graphics,
    juce::TextButton& button,
    bool              shouldDrawButtonAsHighlighted,
    bool              shouldDrawButtonAsDown)
{
  LookAndFeel_V2::drawButtonText(
      graphics,
      button,
      shouldDrawButtonAsHighlighted,
      shouldDrawButtonAsDown);
}

int LookAndFeel::getTextButtonWidthToFitText(
    juce::TextButton& button,
    int               buttonHeight)
{
  return LookAndFeel_V2::getTextButtonWidthToFitText(
      button, buttonHeight);
}

void LookAndFeel::drawDrawableButton(
    juce::Graphics&       graphics,
    juce::DrawableButton& button,
    bool                  shouldDrawButtonAsHighlighted,
    bool                  shouldDrawButtonAsDown)
{
  LookAndFeel_V2::drawDrawableButton(
      graphics,
      button,
      shouldDrawButtonAsHighlighted,
      shouldDrawButtonAsDown);
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

void LookAndFeel::drawSpinningWaitAnimation(
    juce::Graphics&     graphics,
    const juce::Colour& colour,
    int                 x,
    int                 y,
    int                 w,
    int                 h)
{
  LookAndFeel_V2::drawSpinningWaitAnimation(
      graphics, colour, x, y, w, h);
}

void LookAndFeel::drawScrollbarButton(
    juce::Graphics&  graphics,
    juce::ScrollBar& bar,
    int              width,
    int              height,
    int              buttonDirection,
    bool             isScrollbarVertical,
    bool             shouldDrawButtonAsHighlighted,
    bool             shouldDrawButtonAsDown)
{
  LookAndFeel_V2::drawScrollbarButton(
      graphics,
      bar,
      width,
      height,
      buttonDirection,
      isScrollbarVertical,
      shouldDrawButtonAsHighlighted,
      shouldDrawButtonAsDown);
}

juce::ImageEffectFilter* LookAndFeel::getScrollbarEffect()
{
  return LookAndFeel_V2::getScrollbarEffect();
}

int LookAndFeel::getMinimumScrollbarThumbSize(
    juce::ScrollBar& bar)
{
  return LookAndFeel_V2::getMinimumScrollbarThumbSize(
      bar);
}

int LookAndFeel::getScrollbarButtonSize(
    juce::ScrollBar& bar)
{
  return LookAndFeel_V2::getScrollbarButtonSize(
      bar);
}

juce::CaretComponent* LookAndFeel::createCaretComponent(
    juce::Component* keyFocusOwner)
{
  return LookAndFeel_V2::createCaretComponent(
      keyFocusOwner);
}

const juce::Drawable* LookAndFeel::getDefaultFolderImage()
{
  return LookAndFeel_V2::getDefaultFolderImage();
}

const juce::Drawable* LookAndFeel::getDefaultDocumentFileImage()
{
  return LookAndFeel_V2::getDefaultDocumentFileImage();
}

juce::AttributedString LookAndFeel::createFileChooserHeaderText(
    const juce::String& title,
    const juce::String& instructions)
{
  return LookAndFeel_V2::createFileChooserHeaderText(
      title, instructions);
}

void LookAndFeel::drawBubble(
    juce::Graphics&               graphics,
    juce::BubbleComponent&        component,
    const juce::Point<float>&     tip,
    const juce::Rectangle<float>& body)
{
  LookAndFeel_V2::drawBubble(
      graphics, component, tip, body);
}

void LookAndFeel::drawLasso(
    juce::Graphics&  graphics,
    juce::Component& component)
{
  LookAndFeel_V2::drawLasso(
      graphics, component);
}

void LookAndFeel::drawPopupMenuBackgroundWithOptions(
    juce::Graphics&                 graphics,
    int                             width,
    int                             height,
    const juce::PopupMenu::Options& options)
{
  LookAndFeel_V2::drawPopupMenuBackgroundWithOptions(
      graphics, width, height, options);
}

void LookAndFeel::drawPopupMenuItemWithOptions(
    juce::Graphics&                 graphics,
    const juce::Rectangle<int>&     area,
    bool                            isHighlighted,
    const juce::PopupMenu::Item&    item,
    const juce::PopupMenu::Options& options)
{
  LookAndFeel_V2::drawPopupMenuItemWithOptions(
      graphics, area, isHighlighted, item, options);
}

void LookAndFeel::drawPopupMenuSectionHeader(
    juce::Graphics&             graphics,
    const juce::Rectangle<int>& area,
    const juce::String&         sectionName)
{
  LookAndFeel_V2::drawPopupMenuSectionHeader(
      graphics, area, sectionName);
}

void LookAndFeel::drawPopupMenuSectionHeaderWithOptions(
    juce::Graphics&                 graphics,
    const juce::Rectangle<int>&     area,
    const juce::String&             sectionName,
    const juce::PopupMenu::Options& options)
{
  LookAndFeel_V2::drawPopupMenuSectionHeaderWithOptions(
      graphics, area, sectionName, options);
}

juce::Font LookAndFeel::getPopupMenuFont()
{
  return LookAndFeel_V2::getPopupMenuFont();
}

void LookAndFeel::drawPopupMenuUpDownArrow(
    juce::Graphics& graphics,
    int             width,
    int             height,
    bool            isScrollUpArrow)
{
  LookAndFeel_V2::drawPopupMenuUpDownArrow(
      graphics, width, height, isScrollUpArrow);
}

void LookAndFeel::drawPopupMenuUpDownArrowWithOptions(
    juce::Graphics&                 graphics,
    int                             width,
    int                             height,
    bool                            isScrollUpArrow,
    const juce::PopupMenu::Options& options)
{
  LookAndFeel_V2::drawPopupMenuUpDownArrowWithOptions(
      graphics, width, height, isScrollUpArrow, options);
}

void LookAndFeel::getIdealPopupMenuItemSizeWithOptions(
    const juce::String&             text,
    bool                            isSeparator,
    int                             standardMenuItemHeight,
    int&                            idealWidth,
    int&                            idealHeight,
    const juce::PopupMenu::Options& options)
{
  LookAndFeel_V2::getIdealPopupMenuItemSizeWithOptions(
      text,
      isSeparator,
      standardMenuItemHeight,
      idealWidth,
      idealHeight,
      options);
}

int LookAndFeel::getMenuWindowFlags()
{
  return LookAndFeel_V2::getMenuWindowFlags();
}

void LookAndFeel::preparePopupMenuWindow(
    juce::Component& component)
{
  LookAndFeel_V2::preparePopupMenuWindow(
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

juce::Component* LookAndFeel::getParentComponentForMenuOptions(
    const juce::PopupMenu::Options& options)
{
  return LookAndFeel_V2::getParentComponentForMenuOptions(
      options);
}

bool LookAndFeel::shouldPopupMenuScaleWithTargetComponent(
    const juce::PopupMenu::Options& options)
{
  return LookAndFeel_V2::shouldPopupMenuScaleWithTargetComponent(
      options);
}

int LookAndFeel::getPopupMenuBorderSize()
{
  return LookAndFeel_V2::getPopupMenuBorderSize();
}

int LookAndFeel::getPopupMenuBorderSizeWithOptions(
    const juce::PopupMenu::Options& options)
{
  return LookAndFeel_V2::getPopupMenuBorderSizeWithOptions(
      options);
}

void LookAndFeel::drawPopupMenuColumnSeparatorWithOptions(
    juce::Graphics&                 g,
    const juce::Rectangle<int>&     bounds,
    const juce::PopupMenu::Options& options)
{
  LookAndFeel_V2::drawPopupMenuColumnSeparatorWithOptions(
      g, bounds, options);
}

int LookAndFeel::getPopupMenuColumnSeparatorWidthWithOptions(
    const juce::PopupMenu::Options& options)
{
  return LookAndFeel_V2::getPopupMenuColumnSeparatorWidthWithOptions(
      options);
}

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

void LookAndFeel::drawComboBoxTextWhenNothingSelected(
    juce::Graphics& graphics,
    juce::ComboBox& box,
    juce::Label&    label)
{
  LookAndFeel_V2::drawComboBoxTextWhenNothingSelected(
      graphics, box, label);
}

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

void LookAndFeel::drawLinearSliderThumb(
    juce::Graphics&                 graphics,
    int                             x,
    int                             y,
    int                             width,
    int                             height,
    float                           sliderPos,
    float                           minSliderPos,
    float                           maxSliderPos,
    const juce::Slider::SliderStyle style,
    juce::Slider&                   slider)
{
  LookAndFeel_V2::drawLinearSliderThumb(
      graphics,
      x,
      y,
      width,
      height,
      sliderPos,
      minSliderPos,
      maxSliderPos,
      style,
      slider);
}

juce::Button* LookAndFeel::createSliderButton(
    juce::Slider& slider,
    bool          isIncrement)
{
  return LookAndFeel_V2::createSliderButton(
      slider, isIncrement);
}

juce::ImageEffectFilter* LookAndFeel::getSliderEffect(
    juce::Slider& slider)
{
  return LookAndFeel_V2::getSliderEffect(
      slider);
}

juce::Font LookAndFeel::getSliderPopupFont(
    juce::Slider& slider)
{
  return LookAndFeel_V2::getSliderPopupFont(
      slider);
}

int LookAndFeel::getSliderPopupPlacement(
    juce::Slider& slider)
{
  return LookAndFeel_V2::getSliderPopupPlacement(
      slider);
}

juce::Slider::SliderLayout LookAndFeel::getSliderLayout(
    juce::Slider& slider)
{
  return LookAndFeel_V2::getSliderLayout(
      slider);
}

juce::Rectangle<int> LookAndFeel::getTooltipBounds(
    const juce::String&  tipText,
    juce::Point<int>     screenPos,
    juce::Rectangle<int> parentArea)
{
  return LookAndFeel_V2::getTooltipBounds(
      tipText, screenPos, parentArea);
}

juce::Button* LookAndFeel::createFilenameComponentBrowseButton(
    const juce::String& text)
{
  return LookAndFeel_V2::createFilenameComponentBrowseButton(
      text);
}

void LookAndFeel::layoutFilenameComponent(
    juce::FilenameComponent& component,
    juce::ComboBox*          filenameBox,
    juce::Button*            browseButton)
{
  LookAndFeel_V2::layoutFilenameComponent(
      component, filenameBox, browseButton);
}

void LookAndFeel::drawCornerResizer(
    juce::Graphics& graphics,
    int             w,
    int             h,
    bool            isMouseOver,
    bool            isMouseDragging)
{
  LookAndFeel_V2::drawCornerResizer(
      graphics, w, h, isMouseOver, isMouseDragging);
}

void LookAndFeel::drawResizableFrame(
    juce::Graphics&              graphics,
    int                          w,
    int                          h,
    const juce::BorderSize<int>& size)
{
  LookAndFeel_V2::drawResizableFrame(
      graphics, w, h, size);
}

void LookAndFeel::fillResizableWindowBackground(
    juce::Graphics&              graphics,
    int                          w,
    int                          h,
    const juce::BorderSize<int>& size,
    juce::ResizableWindow&       window)
{
  LookAndFeel_V2::fillResizableWindowBackground(
      graphics, w, h, size, window);
}

void LookAndFeel::drawResizableWindowBorder(
    juce::Graphics&              graphics,
    int                          w,
    int                          h,
    const juce::BorderSize<int>& border,
    juce::ResizableWindow&       window)
{
  LookAndFeel_V2::drawResizableWindowBorder(
      graphics, w, h, border, window);
}

juce::DropShadower* LookAndFeel::createDropShadowerForComponent(
    juce::Component* component)
{
  return LookAndFeel_V2::createDropShadowerForComponent(
      component);
}

void LookAndFeel::drawGroupComponentOutline(
    juce::Graphics&            graphics,
    int                        w,
    int                        h,
    const juce::String&        text,
    const juce::Justification& justification,
    juce::GroupComponent&      component)
{
  LookAndFeel_V2::drawGroupComponentOutline(
      graphics, w, h, text, justification, component);
}

int LookAndFeel::getTabButtonBestWidth(
    juce::TabBarButton& button,
    int                 tabDepth)
{
  return LookAndFeel_V2::getTabButtonBestWidth(
      button, tabDepth);
}

juce::Rectangle<int> LookAndFeel::getTabButtonExtraComponentBounds(
    const juce::TabBarButton& button,
    juce::Rectangle<int>&     textArea,
    juce::Component&          extraComp)
{
  return LookAndFeel_V2::getTabButtonExtraComponentBounds(
      button, textArea, extraComp);
}

juce::Font LookAndFeel::getTabButtonFont(
    juce::TabBarButton& button,
    float               height)
{
  return LookAndFeel_V2::getTabButtonFont(
      button, height);
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

void LookAndFeel::drawTabbedButtonBarBackground(
    juce::TabbedButtonBar& bar,
    juce::Graphics&        graphics)
{
  LookAndFeel_V2::drawTabbedButtonBarBackground(
      bar, graphics);
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

juce::Button* LookAndFeel::createTabBarExtrasButton()
{
  return LookAndFeel_V2::createTabBarExtrasButton();
}
void LookAndFeel::drawImageButton(
    juce::Graphics&     graphics,
    juce::Image*        image,
    int                 imageX,
    int                 imageY,
    int                 imageW,
    int                 imageH,
    const juce::Colour& overlayColour,
    float               imageOpacity,
    juce::ImageButton&  button)
{
  LookAndFeel_V2::drawImageButton(
      graphics,
      image,
      imageX,
      imageY,
      imageW,
      imageH,
      overlayColour,
      imageOpacity,
      button);
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

juce::Button* LookAndFeel::createToolbarMissingItemsButton(
    juce::Toolbar& toolbar)
{
  return LookAndFeel_V2::createToolbarMissingItemsButton(
      toolbar);
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

int LookAndFeel::getPropertyPanelSectionHeaderHeight(
    const juce::String& sectionTitle)
{
  return LookAndFeel_V2::getPropertyPanelSectionHeaderHeight(
      sectionTitle);
}

int LookAndFeel::getCallOutBoxBorderSize(
    const juce::CallOutBox& box)
{
  return LookAndFeel_V2::getCallOutBoxBorderSize(
      box);
}

float LookAndFeel::getCallOutBoxCornerSize(
    const juce::CallOutBox& box)
{
  return LookAndFeel_V2::getCallOutBoxCornerSize(
      box);
}

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

void LookAndFeel::drawTableHeaderBackground(
    juce::Graphics&             graphics,
    juce::TableHeaderComponent& component)
{
  LookAndFeel_V3::drawTableHeaderBackground(
      graphics, component);
}

void LookAndFeel::drawTreeviewPlusMinusBox(
    juce::Graphics&               graphics,
    const juce::Rectangle<float>& area,
    juce::Colour                  backgroundColour,
    bool                          isOpen,
    bool                          isMouseOver)
{
  LookAndFeel_V3::drawTreeviewPlusMinusBox(
      graphics, area, backgroundColour, isOpen, isMouseOver);
}

bool LookAndFeel::areLinesDrawnForTreeView(
    juce::TreeView& view)
{
  return LookAndFeel_V3::areLinesDrawnForTreeView(
      view);
}

int LookAndFeel::getTreeViewIndentSize(
    juce::TreeView& view)
{
  return LookAndFeel_V3::getTreeViewIndentSize(
      view);
}

void LookAndFeel::drawKeymapChangeButton(
    juce::Graphics&     graphics,
    int                 width,
    int                 height,
    juce::Button&       button,
    const juce::String& keyDescription)
{
  LookAndFeel_V3::drawKeymapChangeButton(
      graphics, width, height, button, keyDescription);
}

void LookAndFeel::drawPopupMenuBackground(
    juce::Graphics& graphics,
    int             width,
    int             height)
{
  LookAndFeel_V3::drawPopupMenuBackground(
      graphics, width, height);
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

void LookAndFeel::drawTabButton(
    juce::TabBarButton& button,
    juce::Graphics&     graphics,
    bool                isMouseOver,
    bool                isMouseDown)
{
  LookAndFeel_V3::drawTabButton(
      button, graphics, isMouseOver, isMouseDown);
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

bool LookAndFeel::areScrollbarButtonsVisible()
{
  return LookAndFeel_V3::areScrollbarButtonsVisible();
}

void LookAndFeel::drawLinearSliderBackground(
    juce::Graphics&                 graphics,
    int                             x,
    int                             y,
    int                             width,
    int                             height,
    float                           sliderPos,
    float                           minSliderPos,
    float                           maxSliderPos,
    const juce::Slider::SliderStyle style,
    juce::Slider&                   slider)
{
  LookAndFeel_V3::drawLinearSliderBackground(
      graphics,
      x,
      y,
      width,
      height,
      sliderPos,
      minSliderPos,
      maxSliderPos,
      style,
      slider);
}

juce::Button* LookAndFeel::createDocumentWindowButton(
    int i)
{
  return LookAndFeel_V4::createDocumentWindowButton(
      i);
}

void LookAndFeel::positionDocumentWindowButtons(
    juce::DocumentWindow& window,
    int                   i,
    int                   i1,
    int                   i2,
    int                   i3,
    juce::Button*         button,
    juce::Button*         button1,
    juce::Button*         button2,
    bool                  b)
{
  LookAndFeel_V4::positionDocumentWindowButtons(
      window, i, i1, i2, i3, button, button1, button2, b);
}

void LookAndFeel::drawDocumentWindowTitleBar(
    juce::DocumentWindow& window,
    juce::Graphics&       graphics,
    int                   i,
    int                   i1,
    int                   i2,
    int                   i3,
    const juce::Image*    image,
    bool                  b)
{
  LookAndFeel_V4::drawDocumentWindowTitleBar(
      window, graphics, i, i1, i2, i3, image, b);
}

juce::Font LookAndFeel::getTextButtonFont(
    juce::TextButton& button,
    int               buttonHeight)
{
  return LookAndFeel_V4::getTextButtonFont(
      button, buttonHeight);
}

void LookAndFeel::drawButtonBackground(
    juce::Graphics&     graphics,
    juce::Button&       button,
    const juce::Colour& backgroundColour,
    bool                shouldDrawButtonAsHighlighted,
    bool                shouldDrawButtonAsDown)
{
  LookAndFeel_V4::drawButtonBackground(
      graphics,
      button,
      backgroundColour,
      shouldDrawButtonAsHighlighted,
      shouldDrawButtonAsDown);
}

void LookAndFeel::drawToggleButton(
    juce::Graphics&     graphics,
    juce::ToggleButton& button,
    bool                shouldDrawButtonAsHighlighted,
    bool                shouldDrawButtonAsDown)
{
  LookAndFeel_V4::drawToggleButton(
      graphics,
      button,
      shouldDrawButtonAsHighlighted,
      shouldDrawButtonAsDown);
}

void LookAndFeel::drawTickBox(
    juce::Graphics&  graphics,
    juce::Component& component,
    float            x,
    float            y,
    float            w,
    float            h,
    bool             ticked,
    bool             isEnabled,
    bool             shouldDrawButtonAsHighlighted,
    bool             shouldDrawButtonAsDown)
{
  LookAndFeel_V4::drawTickBox(
      graphics,
      component,
      x,
      y,
      w,
      h,
      ticked,
      isEnabled,
      shouldDrawButtonAsHighlighted,
      shouldDrawButtonAsDown);
}

void LookAndFeel::changeToggleButtonWidthToFitText(
    juce::ToggleButton& button)
{
  LookAndFeel_V4::changeToggleButtonWidthToFitText(
      button);
}

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
void LookAndFeel::drawProgressBar(
    juce::Graphics&     graphics,
    juce::ProgressBar&  bar,
    int                 width,
    int                 height,
    double              progress,
    const juce::String& textToShow)
{
  LookAndFeel_V4::drawProgressBar(
      graphics, bar, width, height, progress, textToShow);
}

bool LookAndFeel::isProgressBarOpaque(
    juce::ProgressBar& bar)
{
  return LookAndFeel_V4::isProgressBarOpaque(
      bar);
}

int LookAndFeel::getDefaultScrollbarWidth()
{
  return LookAndFeel_V4::getDefaultScrollbarWidth();
}

void LookAndFeel::drawScrollbar(
    juce::Graphics&  graphics,
    juce::ScrollBar& bar,
    int              x,
    int              y,
    int              width,
    int              height,
    bool             isScrollbarVertical,
    int              thumbStartPosition,
    int              thumbSize,
    bool             isMouseOver,
    bool             isMouseDown)
{
  LookAndFeel_V4::drawScrollbar(
      graphics,
      bar,
      x,
      y,
      width,
      height,
      isScrollbarVertical,
      thumbStartPosition,
      thumbSize,
      isMouseOver,
      isMouseDown);
}

juce::Path LookAndFeel::getTickShape(
    float height)
{
  return LookAndFeel_V4::getTickShape(
      height);
}

juce::Path LookAndFeel::getCrossShape(
    float height)
{
  return LookAndFeel_V4::getCrossShape(
      height);
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

juce::Button* LookAndFeel::createFileBrowserGoUpButton()
{
  return LookAndFeel_V4::createFileBrowserGoUpButton();
}

void LookAndFeel::layoutFileBrowserComponent(
    juce::FileBrowserComponent&              component,
    juce::DirectoryContentsDisplayComponent* display_component,
    juce::FilePreviewComponent*              preview_component,
    juce::ComboBox*                          currentPathBox,
    juce::TextEditor*                        filenameBox,
    juce::Button*                            goUpButton)
{
  LookAndFeel_V4::layoutFileBrowserComponent(
      component,
      display_component,
      preview_component,
      currentPathBox,
      filenameBox,
      goUpButton);
}

void LookAndFeel::drawFileBrowserRow(
    juce::Graphics&                          graphics,
    int                                      width,
    int                                      height,
    const juce::File&                        file,
    const juce::String&                      filename,
    juce::Image*                             icon,
    const juce::String&                      fileSizeDescription,
    const juce::String&                      fileTimeDescription,
    bool                                     isDirectory,
    bool                                     isItemSelected,
    int                                      itemIndex,
    juce::DirectoryContentsDisplayComponent& component)
{
  LookAndFeel_V4::drawFileBrowserRow(
      graphics,
      width,
      height,
      file,
      filename,
      icon,
      fileSizeDescription,
      fileTimeDescription,
      isDirectory,
      isItemSelected,
      itemIndex,
      component);
}

void LookAndFeel::drawPopupMenuItem(
    juce::Graphics&             graphics,
    const juce::Rectangle<int>& area,
    bool                        isSeparator,
    bool                        isActive,
    bool                        isHighlighted,
    bool                        isTicked,
    bool                        hasSubMenu,
    const juce::String&         text,
    const juce::String&         shortcutKeyText,
    const juce::Drawable*       icon,
    const juce::Colour*         textColour)
{
  LookAndFeel_V4::drawPopupMenuItem(
      graphics,
      area,
      isSeparator,
      isActive,
      isHighlighted,
      isTicked,
      hasSubMenu,
      text,
      shortcutKeyText,
      icon,
      textColour);
}

void LookAndFeel::getIdealPopupMenuItemSize(
    const juce::String& text,
    bool                isSeparator,
    int                 standardMenuItemHeight,
    int&                idealWidth,
    int&                idealHeight)
{
  LookAndFeel_V4::getIdealPopupMenuItemSize(
      text,
      isSeparator,
      standardMenuItemHeight,
      idealWidth,
      idealHeight);
}

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

int LookAndFeel::getSliderThumbRadius(
    juce::Slider& slider)
{
  return LookAndFeel_V4::getSliderThumbRadius(
      slider);
}

void LookAndFeel::drawLinearSlider(
    juce::Graphics&                 graphics,
    int                             x,
    int                             y,
    int                             width,
    int                             height,
    float                           sliderPos,
    float                           minSliderPos,
    float                           maxSliderPos,
    const juce::Slider::SliderStyle style,
    juce::Slider&                   slider)
{
  LookAndFeel_V4::drawLinearSlider(
      graphics,
      x,
      y,
      width,
      height,
      sliderPos,
      minSliderPos,
      maxSliderPos,
      style,
      slider);
}

void LookAndFeel::drawRotarySlider(
    juce::Graphics& graphics,
    int             x,
    int             y,
    int             width,
    int             height,
    float           sliderPosProportional,
    float           rotaryStartAngle,
    float           rotaryEndAngle,
    juce::Slider&   slider)
{
  LookAndFeel_V4::drawRotarySlider(
      graphics,
      x,
      y,
      width,
      height,
      sliderPosProportional,
      rotaryStartAngle,
      rotaryEndAngle,
      slider);
}

juce::Label* LookAndFeel::createSliderTextBox(
    juce::Slider& slider)
{
  return LookAndFeel_V4::createSliderTextBox(
      slider);
}

void LookAndFeel::drawTooltip(
    juce::Graphics&     graphics,
    const juce::String& text,
    int                 width,
    int                 height)
{
  LookAndFeel_V4::drawTooltip(
      graphics, text, width, height);
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

void LookAndFeel::drawLevelMeter(
    juce::Graphics& graphics,
    int             i,
    int             i1,
    float           d)
{
  LookAndFeel_V4::drawLevelMeter(
      graphics, i, i1, d);
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

juce::Rectangle<int> LookAndFeel::getPropertyComponentContentPosition(
    juce::PropertyComponent& component)
{
  return LookAndFeel_V4::getPropertyComponentContentPosition(
      component);
}

void LookAndFeel::drawCallOutBoxBackground(
    juce::CallOutBox& box,
    juce::Graphics&   graphics,
    const juce::Path& path,
    juce::Image&      image)
{
  LookAndFeel_V4::drawCallOutBoxBackground(
      box, graphics, path, image);
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

int LookAndFeel::getTooltipSize(
    const juce::String& string,
    int&                i,
    int&                i1)
{
  return juce::TooltipWindow::LookAndFeelMethods::getTooltipSize(
      string,
      i,
      i1);
}

BLOOPER_NAMESPACE_END
