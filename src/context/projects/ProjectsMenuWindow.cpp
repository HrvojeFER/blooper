#include <blooper/context/projects/ProjectsMenuWindow.hpp>


BLOOPER_NAMESPACE_BEGIN

ProjectsMenuWindow::ProjectsMenuWindow(te::Engine& engine)
    : juce::DocumentWindow("Projects", {}, {})
{
    setLookAndFeel(new LookAndFeel());
    setBackgroundColour(
            getLookAndFeel().findColour(
                    juce::ResizableWindow::backgroundColourId));

    setTitleBarButtonsRequired(
            juce::DocumentWindow::allButtons,
            false);

    setContentOwned(
            new ProjectsMenuComponent(engine),
            true);

    const auto monitorArea = getParentMonitorArea();
    centreWithSize(
            monitorArea.getWidth() / 2,
            monitorArea.getHeight() / 2);
}

ProjectsMenuWindow::~ProjectsMenuWindow() = default;

BLOOPER_NAMESPACE_END
