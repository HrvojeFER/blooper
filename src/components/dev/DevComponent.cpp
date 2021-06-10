#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

DevComponent::DevComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}


void DevComponent::resized()
{
}

BLOOPER_NAMESPACE_END
