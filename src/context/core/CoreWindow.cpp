#include <blooper/blooper.hpp>


BLOOPER_NAMESPACE_BEGIN

CoreWindow::CoreWindow(const juce::String& name, CoreContext& context)
    : juce::DocumentWindow(name, {}, {}),
      context(context)
{
    auto& lookAndFeel = context.getLookAndFeel();

    setLookAndFeel(&lookAndFeel);

    setBackgroundColour(lookAndFeel.findColour(
            juce::ResizableWindow::backgroundColourId));


    setTitleBarButtonsRequired(
            juce::DocumentWindow::allButtons,
            false);

    const auto monitorArea = getParentMonitorArea();
    const auto width = monitorArea.getWidth() / 2;
    const auto height = monitorArea.getHeight() / 2;
    centreWithSize(width, height);
}

CoreWindow::~CoreWindow() = default;

BLOOPER_NAMESPACE_END
