#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

[[maybe_unused]] BodyMenuBar::BodyMenuBar(
    AbstractContext& context,
    State            state,
    Options          options)
    : ContextualBase(context),
      StatefulBase(move(state)),
      options(move(options))
{
}


[[maybe_unused]] juce::StringArray BodyMenuBar::getMenuBarNames()
{
  return {"File", "Edit", "View", "Help", "Info", "Dev"};
}

[[maybe_unused]] juce::PopupMenu BodyMenuBar::getMenuForIndex(
    int,
    const juce::String&)
{
  return juce::PopupMenu();
}

[[maybe_unused]] void BodyMenuBar::menuItemSelected(
    int,
    int)
{
}

BLOOPER_NAMESPACE_END
