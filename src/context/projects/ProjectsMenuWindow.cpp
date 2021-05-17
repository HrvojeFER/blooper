#include <blooper/blooper.hpp>


BLOOPER_NAMESPACE_BEGIN

ProjectsMenuWindow::ProjectsMenuWindow(CoreContext& context)
    : CoreWindow("Projects", context),

      onOpen(),
      onCancel()
{
    auto projectsMenu = new ProjectsMenuComponent(context);

    projectsMenu->onOpen = [this](ProjectsMenuComponent::ProjectRef ref) {
        this->onOpen(std::move(ref));
    };

    projectsMenu->onCancel = [this] {
        this->onCancel();
    };

    projectsMenu->setBounds(
            0,
            0,
            getWidth(),
            getHeight());

    setContentOwned(
            projectsMenu,
            true);
}

ProjectsMenuWindow::~ProjectsMenuWindow() = default;


void ProjectsMenuWindow::closeButtonPressed()
{
    this->onCancel();
}


BLOOPER_NAMESPACE_END
