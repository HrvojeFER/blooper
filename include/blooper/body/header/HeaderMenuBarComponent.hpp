#ifndef BLOOPER_HEADER_MENU_BAR_COMPONENT_HPP
#define BLOOPER_HEADER_MENU_BAR_COMPONENT_HPP

#include "blooper/internal/internal.hpp"

BLOOPER_NAMESPACE_BEGIN

class HeaderMenuBarComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(HeaderMenuBarComponent);


  struct Options
  {
  } options;

  explicit HeaderMenuBarComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});


  void resized() override;


 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HeaderMenuBarComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_HEADER_MENU_BAR_COMPONENT_HPP
