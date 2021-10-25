#include <blooper/components/help/HowToComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

HowToComponent::HowToComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}

HowToComponent::~HowToComponent() = default;


// Component

void HowToComponent::resized()
{
}

BLOOPER_NAMESPACE_END
