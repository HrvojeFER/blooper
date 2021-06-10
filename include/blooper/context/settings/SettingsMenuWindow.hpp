#ifndef BLOOPER_SETTINGS_MENU_WINDOW_HPP
#define BLOOPER_SETTINGS_MENU_WINDOW_HPP

#include <blooper/internal/abstract/abstract.hpp>

BLOOPER_NAMESPACE_BEGIN

class SettingsMenuWindow : public CoreWindowBase
{
 public:
  BLOOPER_STATE_ID(SettingsMenuWindowe);


  struct Options
  {
  } options;

  explicit SettingsMenuWindow(
      AbstractCoreContext& context,
      State                state,
      Options              options = {});


  // Window

 private:
  [[maybe_unused]] void closeButtonPressed() override;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsMenuWindow);
};


[[maybe_unused]] SettingsMenuWindow* showSettingsMenu(
    AbstractCoreContext&        context,
    SettingsMenuWindow::Options options = {});

BLOOPER_NAMESPACE_END

#endif // BLOOPER_SETTINGS_MENU_WINDOW_HPP
