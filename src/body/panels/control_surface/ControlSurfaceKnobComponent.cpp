#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

ControlSurfaceKnobComponent::ControlSurfaceKnobComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}


void ControlSurfaceKnobComponent::resized()
{
}

BLOOPER_NAMESPACE_END
