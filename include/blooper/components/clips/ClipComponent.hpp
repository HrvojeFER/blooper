#ifndef BLOOPER_CLIP_COMPONENT_HPP
#define BLOOPER_CLIP_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

struct ClipComponentOptions
{
};

class ClipComponent :
    public ComponentBase
{
 public:
  BLOOPER_STATE_ID(ClipComponent);


  explicit ClipComponent(
      AbstractContext&     context,
      State                state,
      ClipComponentOptions options = {});

  ~ClipComponent() override;

  ClipComponentOptions options;


  // Component

 public:
  void resized() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ClipComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_CLIP_COMPONENT_HPP
