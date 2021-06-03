#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

ControlSurfaceSliderComponent::ControlSurfaceSliderComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}


void ControlSurfaceSliderComponent::resized()
{
}

BLOOPER_NAMESPACE_END
