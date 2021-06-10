#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

DevEngineComponent::DevEngineComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}


void DevEngineComponent::resized()
{
}

BLOOPER_NAMESPACE_END
