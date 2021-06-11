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
    delete this;
  };

  componentOptions.onCancel = [this] {
    this->options.onCancel();
    delete this;
  };

  auto component =
      new ProjectsMenuComponent(
          getContext(),
          getState().getOrCreateChildWithName(
              ProjectsMenuComponent::stateId,
              nullptr),
          componentOptions);

  component->setBounds(this->getBounds());

  setContentOwned(
      component,
      true);
}


// Window

[[maybe_unused]] void ProjectsMenuWindow::closeButtonPressed()
{
  this->options.onCancel();
  delete this;
}


[[maybe_unused]] ProjectsMenuWindow* showProjectsMenu(
    AbstractCoreContext&        context,
    ProjectsMenuWindow::Options options)
{
  ext::visitWindows([](auto window) {
    // idk why this warning
    if (auto projectsWindow = dynamic_cast<ProjectsMenuWindow*>(window))
      delete projectsWindow;
  });

  auto window =
      new ProjectsMenuWindow(
          context,
          context.getState()
              .getOrCreateChildWithName(
                  ProjectsMenuWindow::stateId,
                  nullptr),
          move(options));

  window->setVisible(true);

  return window;
}

BLOOPER_NAMESPACE_END
