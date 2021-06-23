#include <blooper/context/style/LookAndFeel.hpp>

BLOOPER_NAMESPACE_BEGIN

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


void LookAndFeel::preparePopupMenuWindow(
    juce::Component& component)
{
  LookAndFeel_V2::preparePopupMenuWindow(
      component);
}

int LookAndFeel::getMenuWindowFlags()
{
  return LookAndFeel_V2::getMenuWindowFlags();
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


void LookAndFeel::drawPopupMenuBackground(
    juce::Graphics& graphics,
    int             width,
    int             height)
{
  LookAndFeel_V3::drawPopupMenuBackground(
      graphics, width, height);
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


juce::Font LookAndFeel::getPopupMenuFont()
{
  return LookAndFeel_V2::getPopupMenuFont();
}

BLOOPER_NAMESPACE_END
