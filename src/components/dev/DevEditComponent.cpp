#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

DevEditComponent::DevEditComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
    context,
    move(state)),
      options(move(options))
{
}


void DevEditComponent::resized()
{
}

BLOOPER_NAMESPACE_END
