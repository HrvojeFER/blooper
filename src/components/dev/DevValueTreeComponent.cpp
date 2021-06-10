#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

DevValueTreeComponent::DevValueTreeComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}


void DevValueTreeComponent::resized()
{
}

BLOOPER_NAMESPACE_END
