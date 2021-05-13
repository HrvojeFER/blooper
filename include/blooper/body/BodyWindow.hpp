#ifndef BLOOPER_BODY_WINDOW_HPP
#define BLOOPER_BODY_WINDOW_HPP


BLOOPER_NAMESPACE_BEGIN

class BodyWindow : public juce::DocumentWindow
{
public:
    explicit BodyWindow(juce::String name)
        : DocumentWindow(
                  std::move(name),
                  juce::Desktop::getInstance()
                          .getDefaultLookAndFeel()
                          .findColour(juce::ResizableWindow::backgroundColourId),
                  juce::DocumentWindow::allButtons)
    {
        setContentOwned(
                new BodyComponent(),
                true);

#if JUCE_IOS || JUCE_ANDROID
        setFullScreen(true);
#else
        setResizable(
                true,
                true);
        centreWithSize(
                getWidth(),
                getHeight());
#endif

        // this is JUCE code
        setVisible(true);
    }

    void closeButtonPressed() override
    {
        JUCEApplication::getInstance()->systemRequestedQuit();
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BodyWindow)
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_BODY_WINDOW_HPP
