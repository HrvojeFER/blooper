#ifndef BLOOPER_LOOK_AND_FEEL_HPP
#define BLOOPER_LOOK_AND_FEEL_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class LookAndFeel final :
    public CoreContextualBase,
    public StatefulBase,

    public juce::LookAndFeel_V4,

    private util::FlaggedAsyncUpdater
{
 public:
  explicit LookAndFeel(AbstractCoreContext& context);


  // Themes

  [[maybe_unused, nodiscard]] inline const AbstractTheme&
  getTheme() const noexcept;

  [[maybe_unused, nodiscard]] inline AbstractTheme&
  getTheme() noexcept;


  JuceCached<JuceString> themePath;


 private:
  std::unique_ptr<AbstractTheme> theme;
  std::unique_ptr<AbstractTheme> fallback;


  bool themeUpdate;

  void updateTheme();


  bool colourUpdate;

  void updateColours();


  JuceColour getThemeColour(
      JuceColourId id,
      JuceColourId fallbackId);


  // ValueTreeListener

 private:
  void valueTreePropertyChanged(
      juce::ValueTree&        tree,
      const juce::Identifier& id) final;


  // FlaggedAsyncUpdater

 private:
  void handleAsyncUpdate() final;


  // LookAndFeel

 public:
  // Windows

  friend class DocumentWindowButton;

  juce::Button* createDocumentWindowButton(
      int i) final;

  void positionDocumentWindowButtons(
      juce::DocumentWindow& window,
      int                   i,
      int                   i1,
      int                   i2,
      int                   i3,
      juce::Button*         button,
      juce::Button*         button1,
      juce::Button*         button2,
      bool                  b) final;

  void drawDocumentWindowTitleBar(
      juce::DocumentWindow& window,
      juce::Graphics&       graphics,
      int                   i,
      int                   i1,
      int                   i2,
      int                   i3,
      const juce::Image*    image,
      bool                  b) final;

  int getMenuWindowFlags() final;


  void drawCornerResizer(
      juce::Graphics& graphics,
      int             w,
      int             h,
      bool            isMouseOver,
      bool            isMouseDragging) final;

  void drawResizableFrame(
      juce::Graphics&              graphics,
      int                          w,
      int                          h,
      const juce::BorderSize<int>& size) final;

  void fillResizableWindowBackground(
      juce::Graphics&              graphics,
      int                          w,
      int                          h,
      const juce::BorderSize<int>& size,
      juce::ResizableWindow&       window) final;

  void drawResizableWindowBorder(
      juce::Graphics&              graphics,
      int                          w,
      int                          h,
      const juce::BorderSize<int>& border,
      juce::ResizableWindow&       window) final;


  // Popups

  void getIdealPopupMenuItemSize(
      const juce::String& text,
      bool                isSeparator,
      int                 standardMenuItemHeight,
      int&                idealWidth,
      int&                idealHeight) final;

  void drawPopupMenuItem(
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
      const juce::Colour*         textColour) final;

  void drawPopupMenuBackground(
      juce::Graphics& graphics,
      int             width,
      int             height) final;

  void drawPopupMenuBackgroundWithOptions(
      juce::Graphics&                 graphics,
      int                             width,
      int                             height,
      const juce::PopupMenu::Options& options) final;

  void drawPopupMenuItemWithOptions(
      juce::Graphics&                 graphics,
      const juce::Rectangle<int>&     area,
      bool                            isHighlighted,
      const juce::PopupMenu::Item&    item,
      const juce::PopupMenu::Options& options) final;

  void drawPopupMenuSectionHeader(
      juce::Graphics&             graphics,
      const juce::Rectangle<int>& area,
      const juce::String&         sectionName) final;

  void drawPopupMenuSectionHeaderWithOptions(
      juce::Graphics&                 graphics,
      const juce::Rectangle<int>&     area,
      const juce::String&             sectionName,
      const juce::PopupMenu::Options& options) final;

  juce::Font getPopupMenuFont() final;

  void drawPopupMenuUpDownArrow(
      juce::Graphics& graphics,
      int             width,
      int             height,
      bool            isScrollUpArrow) final;

  void drawPopupMenuUpDownArrowWithOptions(
      juce::Graphics&                 graphics,
      int                             width,
      int                             height,
      bool                            isScrollUpArrow,
      const juce::PopupMenu::Options& options) final;

  void getIdealPopupMenuItemSizeWithOptions(
      const juce::String&             text,
      bool                            isSeparator,
      int                             standardMenuItemHeight,
      int&                            idealWidth,
      int&                            idealHeight,
      const juce::PopupMenu::Options& options) final;

  juce::Component* getParentComponentForMenuOptions(
      const juce::PopupMenu::Options& options) final;

  bool shouldPopupMenuScaleWithTargetComponent(
      const juce::PopupMenu::Options& options) final;

  int getPopupMenuBorderSize() final;

  int getPopupMenuBorderSizeWithOptions(
      const juce::PopupMenu::Options& options) final;

  void drawPopupMenuColumnSeparatorWithOptions(
      juce::Graphics&                 g,
      const juce::Rectangle<int>&     bounds,
      const juce::PopupMenu::Options& options) final;

  int getPopupMenuColumnSeparatorWidthWithOptions(
      const juce::PopupMenu::Options& options) final;

  void preparePopupMenuWindow(
      juce::Component& component) final;


  // CallOutBox

  void drawCallOutBoxBackground(
      juce::CallOutBox& box,
      juce::Graphics&   graphics,
      const juce::Path& path,
      juce::Image&      image) final;

  int getCallOutBoxBorderSize(
      const juce::CallOutBox& box) final;

  float getCallOutBoxCornerSize(
      const juce::CallOutBox& box) final;


  // Bubble

  void drawBubble(
      juce::Graphics&               graphics,
      juce::BubbleComponent&        component,
      const juce::Point<float>&     tip,
      const juce::Rectangle<float>& body) final;


  // Tooltips

  juce::Rectangle<int> getTooltipBounds(
      const juce::String&  tipText,
      juce::Point<int>     screenPos,
      juce::Rectangle<int> parentArea) final;

  int getTooltipSize(
      const juce::String& string,
      int&                i,
      int&                i1) final;

  void drawTooltip(
      juce::Graphics&     graphics,
      const juce::String& text,
      int                 width,
      int                 height) final;


  // Alerts

  void playAlertSound() final;

  int getAlertBoxWindowFlags() final;

  juce::AlertWindow* createAlertWindow(
      const juce::String&              title,
      const juce::String&              message,
      const juce::String&              button1,
      const juce::String&              button2,
      const juce::String&              button3,
      juce::AlertWindow::AlertIconType iconType,
      int                              numButtons,
      juce::Component*                 associatedComponent) final;

  void drawAlertBox(
      juce::Graphics&             graphics,
      juce::AlertWindow&          window,
      const juce::Rectangle<int>& textArea,
      juce::TextLayout&           layout) final;

  int getAlertWindowButtonHeight() final;

  juce::Array<int> getWidthsForTextButtons(
      juce::AlertWindow&                    window,
      const juce::Array<juce::TextButton*>& array) final;

  juce::Font getAlertWindowTitleFont() final;

  juce::Font getAlertWindowMessageFont() final;

  juce::Font getAlertWindowFont() final;


  // Progress

  void drawProgressBar(
      juce::Graphics&     graphics,
      juce::ProgressBar&  bar,
      int                 width,
      int                 height,
      double              progress,
      const juce::String& textToShow) final;

  bool isProgressBarOpaque(
      juce::ProgressBar& bar) final;

  void drawSpinningWaitAnimation(
      juce::Graphics&     graphics,
      const juce::Colour& colour,
      int                 x,
      int                 y,
      int                 w,
      int                 h) final;


  // MenuBar

  void drawMenuBarBackground(
      juce::Graphics&         graphics,
      int                     width,
      int                     height,
      bool                    isMouseOverBar,
      juce::MenuBarComponent& component) final;

  void drawMenuBarItem(
      juce::Graphics&         graphics,
      int                     width,
      int                     height,
      int                     itemIndex,
      const juce::String&     itemText,
      bool                    isMouseOverItem,
      bool                    isMenuOpen,
      bool                    isMouseOverBar,
      juce::MenuBarComponent& component) final;

  int getMenuBarItemWidth(
      juce::MenuBarComponent& component,
      int                     itemIndex,
      const juce::String&     itemText) final;

  juce::Font getMenuBarFont(
      juce::MenuBarComponent& component,
      int                     itemIndex,
      const juce::String&     itemText) final;

  int getDefaultMenuBarHeight() final;


  // Toolbar

  juce::Button* createToolbarMissingItemsButton(
      juce::Toolbar& toolbar) final;

  void paintToolbarButtonBackground(
      juce::Graphics&             graphics,
      int                         width,
      int                         height,
      bool                        isMouseOver,
      bool                        isMouseDown,
      juce::ToolbarItemComponent& component) final;

  void paintToolbarBackground(
      juce::Graphics& graphics,
      int             width,
      int             height,
      juce::Toolbar&  toolbar) final;

  void paintToolbarButtonLabel(
      juce::Graphics&             graphics,
      int                         x,
      int                         y,
      int                         width,
      int                         height,
      const juce::String&         text,
      juce::ToolbarItemComponent& component) final;


  // Tabs

  int getTabButtonOverlap(
      int tabDepth) final;

  int getTabButtonSpaceAroundImage() final;

  void drawTabButton(
      juce::TabBarButton& button,
      juce::Graphics&     graphics,
      bool                isMouseOver,
      bool                isMouseDown) final;

  void drawTabAreaBehindFrontButton(
      juce::TabbedButtonBar& bar,
      juce::Graphics&        g,
      int                    w,
      int                    h) final;

  int getTabButtonBestWidth(
      juce::TabBarButton& button,
      int                 tabDepth) final;

  juce::Rectangle<int> getTabButtonExtraComponentBounds(
      const juce::TabBarButton& button,
      juce::Rectangle<int>&     textArea,
      juce::Component&          extraComp) final;

  juce::Font getTabButtonFont(
      juce::TabBarButton& button,
      float               height) final;

  void drawTabButtonText(
      juce::TabBarButton& button,
      juce::Graphics&     graphics,
      bool                isMouseOver,
      bool                isMouseDown) final;

  void drawTabbedButtonBarBackground(
      juce::TabbedButtonBar& bar,
      juce::Graphics&        graphics) final;

  void createTabButtonShape(
      juce::TabBarButton& button,
      juce::Path&         path,
      bool                isMouseOver,
      bool                isMouseDown) final;

  void fillTabButtonShape(
      juce::TabBarButton& button,
      juce::Graphics&     graphics,
      const juce::Path&   path,
      bool                isMouseOver,
      bool                isMouseDown) final;

  juce::Button* createTabBarExtrasButton() final;


  // SidePanel

  juce::Font getSidePanelTitleFont(
      juce::SidePanel& panel) final;

  juce::Justification getSidePanelTitleJustification(
      juce::SidePanel& panel) final;

  juce::Path getSidePanelDismissButtonShape(
      juce::SidePanel& panel) final;


  void drawStretchableLayoutResizerBar(
      juce::Graphics& graphics,
      int             i,
      int             i1,
      bool            b,
      bool            b1,
      bool            b2) final;


  // Scrollbar

  void drawScrollbar(
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
      bool             isMouseDown) final;

  void drawScrollbarButton(
      juce::Graphics&  graphics,
      juce::ScrollBar& bar,
      int              width,
      int              height,
      int              buttonDirection,
      bool             isScrollbarVertical,
      bool             shouldDrawButtonAsHighlighted,
      bool             shouldDrawButtonAsDown) final;

  int getMinimumScrollbarThumbSize(
      juce::ScrollBar& bar) final;

  int getScrollbarButtonSize(
      juce::ScrollBar& bar) final;

  bool areScrollbarButtonsVisible() final;

  int getDefaultScrollbarWidth() final;

  juce::ImageEffectFilter* getScrollbarEffect() final;


  // PropertyPanel

  void drawPropertyPanelSectionHeader(
      juce::Graphics&     graphics,
      const juce::String& name,
      bool                isOpen,
      int                 width,
      int                 height) final;

  void drawPropertyComponentBackground(
      juce::Graphics&          graphics,
      int                      width,
      int                      height,
      juce::PropertyComponent& component) final;

  void drawPropertyComponentLabel(
      juce::Graphics&          graphics,
      int                      width,
      int                      height,
      juce::PropertyComponent& component) final;

  juce::Rectangle<int> getPropertyComponentContentPosition(
      juce::PropertyComponent& component) final;

  int getPropertyPanelSectionHeaderHeight(
      const juce::String& sectionTitle) final;


  void drawConcertinaPanelHeader(
      juce::Graphics&             graphics,
      const juce::Rectangle<int>& area,
      bool                        isMouseOver,
      bool                        isMouseDown,
      juce::ConcertinaPanel&      panel,
      juce::Component&            component) final;


  // Buttons

  void drawButtonBackground(
      juce::Graphics&     graphics,
      juce::Button&       button,
      const juce::Colour& backgroundColour,
      bool                shouldDrawButtonAsHighlighted,
      bool                shouldDrawButtonAsDown) final;


  void drawButtonText(
      juce::Graphics&   graphics,
      juce::TextButton& button,
      bool              shouldDrawButtonAsHighlighted,
      bool              shouldDrawButtonAsDown) final;

  int getTextButtonWidthToFitText(
      juce::TextButton& button,
      int               buttonHeight) final;


  void drawDrawableButton(
      juce::Graphics&       graphics,
      juce::DrawableButton& button,
      bool                  shouldDrawButtonAsHighlighted,
      bool                  shouldDrawButtonAsDown) final;


  void drawImageButton(
      juce::Graphics&     graphics,
      juce::Image*        image,
      int                 imageX,
      int                 imageY,
      int                 imageW,
      int                 imageH,
      const juce::Colour& overlayColour,
      float               imageOpacity,
      juce::ImageButton&  button) final;


  juce::Font getTextButtonFont(
      juce::TextButton& button,
      int               buttonHeight) final;


  void drawToggleButton(
      juce::Graphics&     graphics,
      juce::ToggleButton& button,
      bool                shouldDrawButtonAsHighlighted,
      bool                shouldDrawButtonAsDown) final;

  void drawTickBox(
      juce::Graphics&  graphics,
      juce::Component& component,
      float            x,
      float            y,
      float            w,
      float            h,
      bool             ticked,
      bool             isEnabled,
      bool             shouldDrawButtonAsHighlighted,
      bool             shouldDrawButtonAsDown) final;

  void changeToggleButtonWidthToFitText(
      juce::ToggleButton& button) final;


  void drawKeymapChangeButton(
      juce::Graphics&     graphics,
      int                 width,
      int                 height,
      juce::Button&       button,
      const juce::String& keyDescription) final;


  // Text

  void drawLabel(
      juce::Graphics& graphics,
      juce::Label&    label) final;

  juce::Font getLabelFont(
      juce::Label& label) final;

  juce::BorderSize<int> getLabelBorderSize(
      juce::Label& label) final;


  void fillTextEditorBackground(
      juce::Graphics&   graphics,
      int               width,
      int               height,
      juce::TextEditor& editor) final;

  void drawTextEditorOutline(
      juce::Graphics&   graphics,
      int               width,
      int               height,
      juce::TextEditor& editor) final;


  juce::CaretComponent* createCaretComponent(
      juce::Component* keyFocusOwner) final;


  // Sliders

  void drawLinearSliderBackground(
      juce::Graphics& graphics,
      int             x,
      int             y,
      int             width,
      int             height,
      float           sliderPos,
      float           minSliderPos,
      float           maxSliderPos,
      // NOLINTNEXTLINE(readability-avoid-const-params-in-decls)
      const juce::Slider::SliderStyle style,
      juce::Slider&                   slider) final;

  void drawLinearSliderThumb(
      juce::Graphics& graphics,
      int             x,
      int             y,
      int             width,
      int             height,
      float           sliderPos,
      float           minSliderPos,
      float           maxSliderPos,
      // NOLINTNEXTLINE(readability-avoid-const-params-in-decls)
      const juce::Slider::SliderStyle style,
      juce::Slider&                   slider) final;

  void drawLinearSlider(
      juce::Graphics& graphics,
      int             x,
      int             y,
      int             width,
      int             height,
      float           sliderPos,
      float           minSliderPos,
      float           maxSliderPos,
      // NOLINTNEXTLINE(readability-avoid-const-params-in-decls)
      const juce::Slider::SliderStyle style,
      juce::Slider&                   slider) final;

  void drawRotarySlider(
      juce::Graphics& graphics,
      int             x,
      int             y,
      int             width,
      int             height,
      float           sliderPosProportional,
      float           rotaryStartAngle,
      float           rotaryEndAngle,
      juce::Slider&   slider) final;

  juce::Button* createSliderButton(
      juce::Slider& slider,
      bool          isIncrement) final;

  int getSliderPopupPlacement(
      juce::Slider& slider) final;

  juce::Font getSliderPopupFont(
      juce::Slider& slider) final;

  juce::ImageEffectFilter* getSliderEffect(
      juce::Slider& slider) final;

  juce::Slider::SliderLayout getSliderLayout(
      juce::Slider& slider) final;

  int getSliderThumbRadius(
      juce::Slider& slider) final;

  juce::Label* createSliderTextBox(
      juce::Slider& slider) final;


  void drawLevelMeter(
      juce::Graphics& graphics,
      int             i,
      int             i1,
      float           d) final;


  // ComboBox

  void drawComboBox(
      juce::Graphics& graphics,
      int             width,
      int             height,
      bool            isButtonDown,
      int             buttonX,
      int             buttonY,
      int             buttonW,
      int             buttonH,
      juce::ComboBox& box) final;

  juce::Font getComboBoxFont(
      juce::ComboBox& box) final;

  void positionComboBoxText(
      juce::ComboBox& box,
      juce::Label&    label) final;

  juce::Label* createComboBoxTextBox(
      juce::ComboBox& box) final;

  juce::PopupMenu::Options getOptionsForComboBoxPopupMenu(
      juce::ComboBox& box,
      juce::Label&    label) final;

  void drawComboBoxTextWhenNothingSelected(
      juce::Graphics& graphics,
      juce::ComboBox& box,
      juce::Label&    label) final;


  // TreeView

  void drawTreeviewPlusMinusBox(
      juce::Graphics&               graphics,
      const juce::Rectangle<float>& area,
      juce::Colour                  backgroundColour,
      bool                          isOpen,
      bool                          isMouseOver) final;

  bool areLinesDrawnForTreeView(
      juce::TreeView& view) final;

  int getTreeViewIndentSize(
      juce::TreeView& view) final;


  // Files

  juce::Button* createFileBrowserGoUpButton() final;

  void layoutFileBrowserComponent(
      juce::FileBrowserComponent&              component,
      juce::DirectoryContentsDisplayComponent* display_component,
      juce::FilePreviewComponent*              preview_component,
      juce::ComboBox*                          currentPathBox,
      juce::TextEditor*                        filenameBox,
      juce::Button*                            goUpButton) final;

  void drawFileBrowserRow(
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
      juce::DirectoryContentsDisplayComponent& component) final;

  juce::Button* createFilenameComponentBrowseButton(
      const juce::String& text) final;

  void layoutFilenameComponent(
      juce::FilenameComponent& component,
      juce::ComboBox*          filenameBox,
      juce::Button*            browseButton) final;

  const juce::Drawable* getDefaultDocumentFileImage() final;

  juce::AttributedString createFileChooserHeaderText(
      const juce::String& title,
      const juce::String& instructions) final;

  const juce::Drawable* getDefaultFolderImage() final;


  // Table

  void drawTableHeaderColumn(
      juce::Graphics&             graphics,
      juce::TableHeaderComponent& component,
      const juce::String&         columnName,
      int                         columnId,
      int                         width,
      int                         height,
      bool                        isMouseOver,
      bool                        isMouseDown,
      int                         columnFlags) final;

  void drawTableHeaderBackground(
      juce::Graphics&             graphics,
      juce::TableHeaderComponent& component) final;


  // Grouping

  void drawLasso(
      juce::Graphics&  graphics,
      juce::Component& component) final;

  void drawGroupComponentOutline(
      juce::Graphics&            graphics,
      int                        w,
      int                        h,
      const juce::String&        text,
      const juce::Justification& justification,
      juce::GroupComponent&      component) final;


  // Misc

  std::unique_ptr<juce::LowLevelGraphicsContext> createGraphicsContext(
      const juce::Image&              imageToRenderOn,
      juce::Point<int>                origin,
      const juce::RectangleList<int>& initialClip) final;

  juce::Typeface::Ptr getTypefaceForFont(
      const juce::Font& font) final;

  juce::MouseCursor getMouseCursorFor(
      juce::Component& component) final;

  juce::Path getTickShape(
      float height) final;

  juce::Path getCrossShape(
      float height) final;

  juce::DropShadower* createDropShadowerForComponent(
      juce::Component* component) final;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LookAndFeel)
};


const AbstractTheme& LookAndFeel::getTheme() const noexcept
{
  return *this->theme;
}

AbstractTheme& LookAndFeel::getTheme() noexcept
{
  return *this->theme;
}

BLOOPER_NAMESPACE_END

#endif // BLOOPER_LOOK_AND_FEEL_HPP
