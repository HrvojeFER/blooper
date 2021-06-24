#ifndef BLOOPER_RACK_SLIDER_COMPONENT_HPP
#define BLOOPER_RACK_SLIDER_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

class ControlSurfaceSliderComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(ControlSurfaceSliderComponent);


  struct Options
  {
  } options;

  explicit ControlSurfaceSliderComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});

  ~ControlSurfaceSliderComponent() override;


  // Component

 public:
  void resized() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ControlSurfaceSliderComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_RACK_SLIDER_COMPONENT_HPP
