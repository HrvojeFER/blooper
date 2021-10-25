#ifndef BLOOPER_HELP_COMPONENT_HPP
#define BLOOPER_HELP_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

class HelpComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(HelpComponent);


  struct Options
  {
  } options;

  explicit HelpComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});

  ~HelpComponent() override;


  // Component

 public:
  [[maybe_unused]] void resized() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HelpComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_HELP_COMPONENT_HPP
