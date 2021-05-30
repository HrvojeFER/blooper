#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

HeaderMenuBarComponent::HeaderMenuBarComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}


void HeaderMenuBarComponent::resized()
{
}

BLOOPER_NAMESPACE_END
