#include <blooper/components/clips/ClipComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

ClipComponent::ClipComponent(
    AbstractContext&     context,
    State                state,
    ClipComponentOptions options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}

ClipComponent::~ClipComponent() = default;


// Component

void ClipComponent::resized()
{
}

BLOOPER_NAMESPACE_END
