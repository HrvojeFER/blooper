#ifndef BLOOPER_HOW_TO_COMPONENT_HPP
#define BLOOPER_HOW_TO_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

class HowToComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(HowToComponent);


  struct Options
  {
  } options;

  explicit HowToComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});

  ~HowToComponent() override;


  // Component

 public:
  [[maybe_unused]] void resized() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HowToComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_HOW_TO_COMPONENT_HPP
