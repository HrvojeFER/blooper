#include <blooper/components/help/DocsComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

DocsComponent::DocsComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}

DocsComponent::~DocsComponent() = default;


// Component

void DocsComponent::resized()
{
}

BLOOPER_NAMESPACE_END
