#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

ControlSurfacePanelComponent::ControlSurfacePanelComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}


void ControlSurfacePanelComponent::resized()
{
}

BLOOPER_NAMESPACE_END
