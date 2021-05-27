#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

RackKnobComponent::RackKnobComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
    context,
    std::move(state)),
      options(std::move(options))
{
}


void RackKnobComponent::resized()
{
}

BLOOPER_NAMESPACE_END
