#include <blooper/body/panels/control_surface/ControlSurfaceSliderComponent.hpp>

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

ControlSurfaceSliderComponent::~ControlSurfaceSliderComponent() = default;


// Component

void ControlSurfaceSliderComponent::resized()
{
}

BLOOPER_NAMESPACE_END
