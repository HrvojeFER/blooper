#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

ProjectComponent::ProjectComponent(
    AbstractContext& context,
    State            state,
    JuceProjectRef   project,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options)),
      project(move(project))
{
  if (!project) project = getContext().getProjectRef();
}

BLOOPER_NAMESPACE_END
