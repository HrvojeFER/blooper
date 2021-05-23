#ifndef BLOOPER_DEVICES_MENU_WINDOW_HPP
#define BLOOPER_DEVICES_MENU_WINDOW_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class DevicesMenuWindow : public CoreWindowBase
{
 public:
  BLOOPER_STATE_ID(DevicesMenuWindow);


  struct Options
  {
  } options;

  explicit DevicesMenuWindow(
      AbstractCoreContext& context,
      State                state,
      Options              options = {});


 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DevicesMenuWindow);
};


[[maybe_unused]] void showDevicesMenu(
    AbstractCoreContext&       context,
    DevicesMenuWindow::Options options = {});

BLOOPER_NAMESPACE_END

#endif // BLOOPER_DEVICES_MENU_WINDOW_HPP
