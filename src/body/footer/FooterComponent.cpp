#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

FooterComponent::FooterComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          std::move(state)),
      options(std::move(options))
{
}


void FooterComponent::resized()
{
}

BLOOPER_NAMESPACE_END
