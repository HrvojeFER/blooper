#ifndef BLOOPER_PANEL_WINDOW_HPP
#define BLOOPER_PANEL_WINDOW_HPP

#include <blooper/internal/internal.hpp>

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


  void closeButtonPressed() override;


 private:
  JuceComponent& content;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PanelWindow)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PANEL_WINDOW_HPP
