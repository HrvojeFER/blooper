#ifndef BLOOPER_RACK_KNOB_COMPONENT_HPP
#define BLOOPER_RACK_KNOB_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class RackKnobComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(RackKnobComponent);


  struct Options
  {
  } options;

  explicit RackKnobComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});


  void resized() override;


 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RackKnobComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_RACK_KNOB_COMPONENT_HPP
