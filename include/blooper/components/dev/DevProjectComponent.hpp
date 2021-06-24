#ifndef BLOOPER_DEV_PROJECT_COMPONENT_HPP
#define BLOOPER_DEV_PROJECT_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

class DevProjectComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(DevProjectComponent);


  struct Options
  {
  } options;

  explicit DevProjectComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});

  ~DevProjectComponent() override;


  // Component

 public:
  [[maybe_unused]] void resized() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DevProjectComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_DEV_PROJECT_COMPONENT_HPP
