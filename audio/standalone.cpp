#include <blooper/blooper.hpp>


namespace blooper
{
class app : public audio::JUCEApplication
{
public:
    app() = default;

    const juce::String getApplicationName() override // NOLINT(readability-const-return-type)
    {
        return JUCE_APPLICATION_NAME_STRING;
    }
    const juce::String getApplicationVersion() override // NOLINT(readability-const-return-type)
    {
        return JUCE_APPLICATION_VERSION_STRING;
    }
    bool moreThanOneInstanceAllowed() override
    {
        return true;
    }

    void initialise(const juce::String&) override
    {
        if (audio::RuntimePermissions::isRequired(
                    juce::RuntimePermissions::recordAudio) &&
            !audio::RuntimePermissions::isGranted(
                    juce::RuntimePermissions::recordAudio))
        {
            audio::RuntimePermissions::request(
                    juce::RuntimePermissions::recordAudio,
                    [this](bool is_granted) {
                        if (!is_granted)
                        {
                            this->systemRequestedQuit();
                        }
                    });
        }

        _window = std::make_unique<window>(getApplicationName());
    }

    void shutdown() override
    {
        _window = nullptr;
    }

    [[maybe_unused]] void systemRequestedQuit() override
    {
        quit();
    }

    void anotherInstanceStarted(const juce::String&) override
    {
    }

    class window : public juce::DocumentWindow
    {
    public:
        explicit window(const juce::String& name)
            : DocumentWindow(
                      name,
                      juce::Desktop::getInstance()
                              .getDefaultLookAndFeel()
                              .findColour(
                                      ResizableWindow::backgroundColourId),
                      DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(
                    new main_component(),
                    true);

#if JUCE_IOS || JUCE_ANDROID
            setFullScreen(true);
#else
            setResizable(true, true);
            centreWithSize(getWidth(), getHeight());
#endif

            // this is from JUCE, so it should work i guess...
            setVisible(true);
        }

        void closeButtonPressed() override
        {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(window)
    };

private:
    std::unique_ptr<window> _window;
};
} // namespace blooper


START_JUCE_APPLICATION(blooper::app)
