#ifndef BLOOPER_INFO_WINDOW_HPP
#define BLOOPER_INFO_WINDOW_HPP
#pragma once

#include <blooper/internal/abstract/windows.hpp>

BLOOPER_NAMESPACE_BEGIN

class InfoWindow : public WindowBase
{
 public:
  BLOOPER_STATE_ID(InfoWindow);


  struct Options
  {
  } options;

  explicit InfoWindow(
      AbstractContext& context,
      State            state,
      Options          options = {});

  ~InfoWindow() override;


  // Window

 private:
  [[maybe_unused]] void closeButtonPressed() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InfoWindow);
};


// Show

[[maybe_unused]] InfoWindow* showInfo(
    AbstractContext&    context,
    InfoWindow::Options options = {});

BLOOPER_NAMESPACE_END

#endif // BLOOPER_INFO_WINDOW_HPP
