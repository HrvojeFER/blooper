#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

HeaderToolbarComponent::HeaderToolbarComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}


void HeaderToolbarComponent::resized()
{
}

BLOOPER_NAMESPACE_END
