#include <blooper/context/projects/ProjectsMenuWindow.hpp>


BLOOPER_NAMESPACE_BEGIN

ProjectsMenuWindow::ProjectsMenuWindow(te::Engine& engine)
    : juce::DocumentWindow("Projects", {}, {}),

      engine(engine),

      lookAndFeel(std::make_unique<LookAndFeel>()),

      onOpen(),
      onCancel()
{
    setLookAndFeel(&lookAndFeel);
    setBackgroundColour(
            getLookAndFeel().findColour(
                    juce::ResizableWindow::backgroundColourId));

    setTitleBarButtonsRequired(
            juce::DocumentWindow::allButtons,
            false);


    const auto monitorArea = getParentMonitorArea();
    const auto width = monitorArea.getWidth() / 2;
    const auto height = monitorArea.getHeight() / 2;
    centreWithSize(width, height);


    auto projectsMenu = new ProjectsMenuComponent(engine);

    projectsMenu->onOpen = [this](ProjectsMenuComponent::ProjectRef ref) {
        this->onOpen(std::move(ref));
    };

    projectsMenu->onCancel = [this] {
        this->onCancel();
    };

    projectsMenu->setBounds(0, 0, width, height);

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
