#ifndef BLOOPER_DEV_WINDOW_HPP
#define BLOOPER_DEV_WINDOW_HPP
#pragma once

#include <blooper/internal/abstract/windows.hpp>

BLOOPER_NAMESPACE_BEGIN

class DevWindow : public WindowBase
{
 public:
  BLOOPER_STATE_ID(DevWindow);


  struct Options
  {
  } options;

  explicit DevWindow(
      AbstractContext& context,
      State            state,
      Options          options = {});

  ~DevWindow() override;


  // Window

 private:
  [[maybe_unused]] void closeButtonPressed() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DevWindow);
};


// Show

[[maybe_unused]] DevWindow* showDev(
    AbstractContext&   context,
    DevWindow::Options options = {});

BLOOPER_NAMESPACE_END

#endif // BLOOPER_DEV_WINDOW_HPP
