#ifndef BLOOPER_BODY_WINDOW_MENU_BAR_COMPONENT_HPP
#define BLOOPER_BODY_WINDOW_MENU_BAR_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class BodyMenuBarComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(BodyMenuBarComponent);


  struct Options
  {
  } options;

  explicit BodyMenuBarComponent(
      AbstractContext& context,
      State            state,
      Options          options);


  void resized() override;


 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BodyMenuBarComponent);
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_BODY_WINDOW_MENU_BAR_COMPONENT_HPP
