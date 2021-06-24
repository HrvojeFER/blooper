#ifndef BLOOPER_HELP_WINDOW_HPP
#define BLOOPER_HELP_WINDOW_HPP
#pragma once

#include <blooper/internal/abstract/windows.hpp>

BLOOPER_NAMESPACE_BEGIN

class HelpWindow : public WindowBase
{
 public:
  BLOOPER_STATE_ID(HelpWindow);


  struct Options
  {
  } options;

  explicit HelpWindow(
      AbstractContext& context,
      State            state,
      Options          options = {});

  ~HelpWindow() override;


  // Window

 private:
  [[maybe_unused]] void closeButtonPressed() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HelpWindow);
};


// Show

[[maybe_unused]] HelpWindow* showHelp(
    AbstractContext&    context,
    HelpWindow::Options options = {});

BLOOPER_NAMESPACE_END

#endif // BLOOPER_HELP_WINDOW_HPP
