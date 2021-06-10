#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

InfoComponent::InfoComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}


void InfoComponent::resized()
{
}

BLOOPER_NAMESPACE_END
