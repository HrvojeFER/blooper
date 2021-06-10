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

    if (this->isCurrentlyModal())
      this->exitModalState(0);
  };

  componentOptions.onCancel = [this] {
    this->options.onCancel();

    if (this->isCurrentlyModal())
      this->exitModalState(0);
  };

  auto component =
      new ProjectsMenuComponent(
          getContext(),
          getState().getOrCreateChildWithName(
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


// Window

[[maybe_unused]] void ProjectsMenuWindow::closeButtonPressed()
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
