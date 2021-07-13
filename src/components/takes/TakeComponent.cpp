#include <blooper/components/takes/TakeComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

TakeComponent::TakeComponent(
    AbstractContext&     context,
    State                state,
    TakeComponentOptions options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}

TakeComponent::~TakeComponent() = default;


// Component

void TakeComponent::resized()
{
}

BLOOPER_NAMESPACE_END
