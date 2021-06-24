#ifndef BLOOPER_BODY_WINDOW_MENU_BAR_COMPONENT_HPP
#define BLOOPER_BODY_WINDOW_MENU_BAR_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/stateful.hpp>
#include <blooper/internal/abstract/contextual.hpp>

BLOOPER_NAMESPACE_BEGIN

class BodyMenuBar :
    public ContextualBase,
    public StatefulBase,

    public juce::MenuBarModel
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

 public:
  [[maybe_unused]] juce::StringArray getMenuBarNames() override;

  [[maybe_unused]] juce::PopupMenu getMenuForIndex(
      int                 topLevelMenuIndex,
      const juce::String& menuName) override;

  [[maybe_unused]] void menuItemSelected(
      int menuItemID,
      int topLevelMenuIndex) override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BodyMenuBar);
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_BODY_WINDOW_MENU_BAR_COMPONENT_HPP
