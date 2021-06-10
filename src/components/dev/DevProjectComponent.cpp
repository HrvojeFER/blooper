#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

DevProjectComponent::DevProjectComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}


void DevProjectComponent::resized()
{
}

BLOOPER_NAMESPACE_END
