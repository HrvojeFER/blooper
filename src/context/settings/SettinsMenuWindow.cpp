#include <blooper/context/settings/SettingsMenuWindow.hpp>


BLOOPER_NAMESPACE_BEGIN

SettingsMenuWindow::SettingsMenuWindow(te::Engine& engine)
    : juce::DocumentWindow("Settings", {}, {}),
      engine(engine)
{
    setLookAndFeel(new LookAndFeel());
    setBackgroundColour(
            getLookAndFeel().findColour(
                    juce::ResizableWindow::backgroundColourId));

    setTitleBarButtonsRequired(
            juce::DocumentWindow::allButtons,
            false);

    setContentOwned(
            new SettingsMenuComponent(engine),
            true);

    const auto monitorArea = getParentMonitorArea();
    centreWithSize(
            monitorArea.getWidth() / 2,
            monitorArea.getHeight() / 2);
}

SettingsMenuWindow::~SettingsMenuWindow() = default;

BLOOPER_NAMESPACE_END
