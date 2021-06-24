#ifndef BLOOPER_FOOTER_COMPONENT_HPP
#define BLOOPER_FOOTER_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

class FooterComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(FooterComponent);


  struct Options
  {
  } options;

  explicit FooterComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});

  ~FooterComponent() override;


  // Component

 public:
  void resized() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FooterComponent)
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_FOOTER_COMPONENT_HPP
