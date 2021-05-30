#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

FooterComponent::FooterComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}


void FooterComponent::resized()
{
}

BLOOPER_NAMESPACE_END
