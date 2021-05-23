#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

ProjectsMenuWindow::ProjectsMenuWindow(
    AbstractCoreContext& context,
    State                state,
    Options              options)
    : CoreWindowBase(
          "Projects",
          context,
          std::move(state)),

      options(std::move(options))
{
  auto component =
      new ProjectsMenuComponent(
          context,
          state.getOrCreateChildWithName(
              ProjectsMenuComponent::stateId,
              nullptr));

  component->onOpen = [this](auto ref) {
    this->options.onOpen(std::move(ref));
  };

  component->onCancel = [this] {
    this->options.onCancel();
  };

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


[[maybe_unused]] void showProjectsMenu(
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
          std::move(options));

  window->enterModalState(
      true,
      nullptr,
      true);
}

BLOOPER_NAMESPACE_END
