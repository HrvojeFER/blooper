#include <blooper/body/panels/control_surface/ControlSurfaceKnobComponent.hpp>

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

ControlSurfaceKnobComponent::~ControlSurfaceKnobComponent() = default;


// Component

void ControlSurfaceKnobComponent::resized()
{
}

BLOOPER_NAMESPACE_END
