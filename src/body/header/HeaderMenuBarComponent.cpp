#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

HeaderMenuBarComponent::HeaderMenuBarComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          std::move(state)),
      options(std::move(options))
{
}


void HeaderMenuBarComponent::resized()
{
}

BLOOPER_NAMESPACE_END
