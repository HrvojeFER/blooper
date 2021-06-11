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
          move(state)),
      options(move(options)),
      project(move(project))
{
  if (!project) project = getContext().getProjectRef();


  ProjectComponent::Options componentOptions{};

  auto component = new ProjectComponent(
      getContext(),
      getState().getOrCreateChildWithName(
          ProjectsMenuComponent::stateId,
          nullptr),
      project,
      move(componentOptions));

  component->setBounds(this->getBounds());

  setContentOwned(component, true);
}


// Window

[[maybe_unused]] void ProjectWindow::closeButtonPressed()
{
  delete this;
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
      move(project),
      move(options));

  window->enterModalState(
      true,
      nullptr,
      true);

  return window;
}

BLOOPER_NAMESPACE_END
