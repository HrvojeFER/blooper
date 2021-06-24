#ifndef BLOOPER_HEADER_MENU_BAR_COMPONENT_HPP
#define BLOOPER_HEADER_MENU_BAR_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

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

  ~HeaderMenuBarComponent() override;


  // Component

 public:
  void resized() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HeaderMenuBarComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_HEADER_MENU_BAR_COMPONENT_HPP
