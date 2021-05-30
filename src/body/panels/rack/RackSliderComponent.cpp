#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

RackSliderComponent::RackSliderComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}


void RackSliderComponent::resized()
{
}

BLOOPER_NAMESPACE_END
