#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

RackPanelComponent::RackPanelComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          std::move(state)),
      options(std::move(options))
{
}


void RackPanelComponent::resized()
{
}

BLOOPER_NAMESPACE_END
