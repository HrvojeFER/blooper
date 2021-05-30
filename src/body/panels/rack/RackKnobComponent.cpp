#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

RackKnobComponent::RackKnobComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
    context,
    move(state)),
      options(move(options))
{
}


void RackKnobComponent::resized()
{
}

BLOOPER_NAMESPACE_END
