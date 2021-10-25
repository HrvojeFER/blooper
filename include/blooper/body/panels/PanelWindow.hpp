#ifndef BLOOPER_PANEL_WINDOW_HPP
#define BLOOPER_PANEL_WINDOW_HPP
#pragma once

#include <blooper/internal/abstract/windows.hpp>

BLOOPER_NAMESPACE_BEGIN

class PanelWindow : public WindowBase
{
 public:
  BLOOPER_STATE_ID(PanelsComponent);


  struct Options
  {
    std::function<void()> onClose;
  } options;

  explicit PanelWindow(
      AbstractContext& context,
      State            state,
      JuceString       name,
      JuceComponent&   content,
      Options          options = {});

  ~PanelWindow() override;


 private:
  JuceComponent& content;


  // Window

 public:
  void closeButtonPressed() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PanelWindow)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PANEL_WINDOW_HPP
