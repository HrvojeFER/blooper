#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

HeaderToolbarComponent::HeaderToolbarComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          std::move(state)),
      options(std::move(options))
{
}


void HeaderToolbarComponent::resized()
{
}

BLOOPER_NAMESPACE_END
