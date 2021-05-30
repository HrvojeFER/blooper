#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

ProjectsMenuWindow::ProjectsMenuWindow(
    AbstractCoreContext& context,
    State                state,
    Options              options)
    : CoreWindowBase(
          "Projects",
          context,
          move(state)),

      options(move(options))
{
  ProjectsMenuComponent::Options componentOptions{};

  componentOptions.onOpen = [this](auto ref) {
    this->options.onOpen(move(ref));
  };

  componentOptions.onCancel = [this] {
    this->options.onCancel();
  };

  auto component =
      new ProjectsMenuComponent(
          getContext(),
          state.getOrCreateChildWithName(
              ProjectsMenuComponent::stateId,
              nullptr),
          componentOptions);

  component->setBounds(
      0,
      0,
      this->getWidth(),
      this->getHeight());

  setContentOwned(
      component,
      true);
}


void ProjectsMenuWindow::closeButtonPressed()
{
  this->options.onCancel();
}


[[maybe_unused]] ProjectsMenuWindow* showProjectsMenu(
    AbstractCoreContext&        context,
    ProjectsMenuWindow::Options options)
{
  auto window =
      new ProjectsMenuWindow(
          context,
          context.getState()
              .getOrCreateChildWithName(
                  ProjectsMenuWindow::stateId,
                  nullptr),
          move(options));

  window->enterModalState(
      true,
      nullptr,
      true);

  return window;
}

BLOOPER_NAMESPACE_END
