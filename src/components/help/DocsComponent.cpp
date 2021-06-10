#include <blooper/blooper.hpp>

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


void DocsComponent::resized()
{
}

BLOOPER_NAMESPACE_END
