#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

PanelsComponent::PanelsComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          std::move(state)),
      options(std::move(options))
{
}


void PanelsComponent::resized()
{
}

BLOOPER_NAMESPACE_END
