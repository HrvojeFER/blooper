#ifndef BLOOPER_DEV_VALUE_TREE_COMPONENT_HPP
#define BLOOPER_DEV_VALUE_TREE_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

class DevValueTreeComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(DevValueTreeComponent);


  struct Options
  {
  } options;

  explicit DevValueTreeComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});

  ~DevValueTreeComponent() override;


  // Component

 public:
  [[maybe_unused]] void resized() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DevValueTreeComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_DEV_VALUE_TREE_COMPONENT_HPP
