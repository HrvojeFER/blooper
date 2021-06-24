#include <blooper/components/help/TutorialComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

TutorialComponent::TutorialComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}

TutorialComponent::~TutorialComponent() = default;


// Component

void TutorialComponent::resized()
{
}

BLOOPER_NAMESPACE_END
