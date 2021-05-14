#include <blooper/body/BodyWindow.hpp>


BLOOPER_NAMESPACE_BEGIN

BodyWindow::BodyWindow()
    : DocumentWindow(
              juce::String(JUCE_APPLICATION_NAME_STRING) +
                      juce::String(" ") +
                      juce::String(JUCE_APPLICATION_VERSION_STRING),
              {},
              {}),
      onClose([] {})
{
    setLookAndFeel(
            new LookAndFeel());

    setBackgroundColour(
            getLookAndFeel().findColour(
                    juce::ResizableWindow::backgroundColourId));

    setTitleBarButtonsRequired(
            juce::DocumentWindow::allButtons,
            false);


    setContentOwned(
            new BodyComponentOld(),
            true);


    setFullScreen(true);
}

BodyWindow::~BodyWindow() = default;


void BodyWindow::closeButtonPressed()
{
    onClose();
}

BLOOPER_NAMESPACE_END
