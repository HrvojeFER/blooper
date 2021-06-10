#ifndef BLOOPER_PLUGINS_MENU_WINDOW_HPP
#define BLOOPER_PLUGINS_MENU_WINDOW_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class PluginsMenuWindow : public CoreWindowBase
{
 public:
  BLOOPER_STATE_ID(PluginsMenuWindow);


  struct Options
  {
  } options;

  explicit PluginsMenuWindow(
      AbstractCoreContext& context,
      State                state,
      Options              options = {});


  // Window

 private:
  [[maybe_unused]] void closeButtonPressed() override;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginsMenuWindow)
};


[[maybe_unused]] PluginsMenuWindow* showPluginsMenu(
    AbstractCoreContext&       context,
    PluginsMenuWindow::Options options = {});

BLOOPER_NAMESPACE_END


#endif // BLOOPER_PLUGINS_MENU_WINDOW_HPP
