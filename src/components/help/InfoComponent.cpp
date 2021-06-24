#include <blooper/components/help/InfoComponent.hpp>

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

InfoComponent::~InfoComponent() = default;


// Component

void InfoComponent::resized()
{
}

BLOOPER_NAMESPACE_END
