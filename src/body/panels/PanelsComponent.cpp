#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

PanelsComponent::PanelsComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}


void PanelsComponent::resized()
{
}

BLOOPER_NAMESPACE_END
