#ifndef BLOOPER_TAKE_COMPONENT_HPP
#define BLOOPER_TAKE_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

struct TakeComponentOptions
{
};

class TakeComponent :
    public ComponentBase
{
 public:
  BLOOPER_STATE_ID(TakeComponent);


  explicit TakeComponent(
      AbstractContext&     context,
      State                state,
      TakeComponentOptions options = {});

  ~TakeComponent() override;

  TakeComponentOptions options;


  // Component

 public:
  void resized() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TakeComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_TAKE_COMPONENT_HPP
