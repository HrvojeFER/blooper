#ifndef BLOOPER_BODY_WINDOW_HPP
#define BLOOPER_BODY_WINDOW_HPP
#pragma once

#include <blooper/internal/abstract/windows.hpp>

BLOOPER_NAMESPACE_BEGIN

class BodyWindow : public WindowBase
{
 public:
  BLOOPER_STATE_ID(BodyWindow);


  struct Options
  {
    std::function<void()> onClose;
  } options;

  explicit BodyWindow(
      AbstractContext& context,
      State            state,
      JuceString       name,
      Options          options = {});

  ~BodyWindow() override;


 private:
  std::unique_ptr<class BodyComponent> component;

  std::unique_ptr<class BodyMenuBar> menuBar;


  // Window

 private:
  [[maybe_unused]] void closeButtonPressed() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BodyWindow);
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_BODY_WINDOW_HPP
