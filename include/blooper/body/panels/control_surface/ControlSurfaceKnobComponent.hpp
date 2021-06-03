#ifndef BLOOPER_RACK_KNOB_COMPONENT_HPP
#define BLOOPER_RACK_KNOB_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class ControlSurfaceKnobComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(ControlSurfaceKnobComponent);


  struct Options
  {
  } options;

  explicit ControlSurfaceKnobComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});


  void resized() override;


 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ControlSurfaceKnobComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_RACK_KNOB_COMPONENT_HPP
