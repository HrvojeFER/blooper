#ifndef BLOOPER_RACK_SLIDER_COMPONENT_HPP
#define BLOOPER_RACK_SLIDER_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class RackSliderComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(RackSliderComponent);


  struct Options
  {
  } options;

  explicit RackSliderComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});


  void resized() override;


 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RackSliderComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_RACK_SLIDER_COMPONENT_HPP
