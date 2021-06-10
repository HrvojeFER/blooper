#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

HowToComponent::HowToComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}


void HowToComponent::resized()
{
}

BLOOPER_NAMESPACE_END
