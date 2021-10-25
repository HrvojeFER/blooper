#ifndef BLOOPER_DEVICES_MENU_WINDOW_HPP
#define BLOOPER_DEVICES_MENU_WINDOW_HPP
#pragma once

#include <blooper/internal/abstract/windows.hpp>

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


  // Window

 private:
  [[maybe_unused]] void closeButtonPressed() override;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DevicesMenuWindow);
};


[[maybe_unused]] DevicesMenuWindow* showDevicesMenu(
    AbstractCoreContext&       context,
    DevicesMenuWindow::Options options = {});

BLOOPER_NAMESPACE_END

#endif // BLOOPER_DEVICES_MENU_WINDOW_HPP
