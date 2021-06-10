#ifndef BLOOPER_BODY_WINDOW_MENU_BAR_COMPONENT_HPP
#define BLOOPER_BODY_WINDOW_MENU_BAR_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class BodyMenuBar :
    public ContextualBase,
    public StatefulBase,

    private juce::MenuBarModel
{
 public:
  BLOOPER_STATE_ID(BodyMenuBar);


  struct Options
  {
  } options;

  [[maybe_unused]] explicit BodyMenuBar(
      AbstractContext& context,
      State            state,
      Options          options);


  // MenuBarModel

 private:
  friend class juce::MenuBarComponent;
  friend class BodyWindow;

  [[maybe_unused]] juce::StringArray getMenuBarNames() override;

  [[maybe_unused]] juce::PopupMenu getMenuForIndex(
      int                 topLevelMenuIndex,
      const juce::String& menuName) override;

  [[maybe_unused]] void menuItemSelected(
      int menuItemID,
      int topLevelMenuIndex) override;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BodyMenuBar);
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_BODY_WINDOW_MENU_BAR_COMPONENT_HPP
