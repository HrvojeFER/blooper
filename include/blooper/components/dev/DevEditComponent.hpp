#ifndef BLOOPER_DEV_EDIT_COMPONENT_HPP
#define BLOOPER_DEV_EDIT_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

class DevEditComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(DevEditComponent);


  struct Options
  {
  } options;

  explicit DevEditComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});

  ~DevEditComponent() override;


  // Component

 public:
  [[maybe_unused]] void resized() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DevEditComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_DEV_EDIT_COMPONENT_HPP
