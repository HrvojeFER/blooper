#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

ProjectComponent::ProjectComponent(
    AbstractContext& context,
    State            state,
    JuceProjectRef   project,
    Options          options)
    : ComponentBase(
          context,
          std::move(state)),
      options(std::move(options)),
      project(std::move(project))
{
  if (!project) project = getContext().getProjectRef();
}

BLOOPER_NAMESPACE_END
