#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

RackSliderComponent::RackSliderComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          std::move(state)),
      options(std::move(options))
{
}


void RackSliderComponent::resized()
{
}

BLOOPER_NAMESPACE_END
