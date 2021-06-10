#include <blooper/blooper.hpp>

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


void TutorialComponent::resized()
{
}

BLOOPER_NAMESPACE_END
