#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

ProjectWindow::ProjectWindow(
    AbstractContext& context,
    State            state,
    JuceProjectRef   project,
    Options          options)
    : WindowBase(
          "Project",
          context,
          std::move(state)),
      options(std::move(options)),
      project(std::move(project))
{
  if (!project) project = getContext().getProjectRef();


  ProjectComponent::Options componentOptions{};

  setContentOwned(
      new ProjectComponent(
          getContext(),
          getState().getOrCreateChildWithName(
              ProjectsMenuComponent::stateId,
              nullptr),
          project,
          std::move(componentOptions)),
      true);
}


[[maybe_unused]] ProjectWindow* showProject(
    AbstractContext&       context,
    JuceProjectRef         project,
    ProjectWindow::Options options)
{
  auto window = new ProjectWindow(
      context,
      context.getState().getOrCreateChildWithName(
          ProjectWindow::stateId,
          nullptr),
      std::move(project),
      std::move(options));

  window->enterModalState(
      true,
      nullptr,
      true);

  return window;
}

BLOOPER_NAMESPACE_END
