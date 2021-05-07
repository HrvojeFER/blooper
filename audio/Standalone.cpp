#include <blooper/MainComponent.hpp>


namespace blooper
{
class App : public juce::JUCEApplication
{
public:
    App() = default;

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
        if (juce::RuntimePermissions::isRequired(
                    juce::RuntimePermissions::recordAudio) &&
            !juce::RuntimePermissions::isGranted(
                    juce::RuntimePermissions::recordAudio))
        {
            juce::RuntimePermissions::request(
                    juce::RuntimePermissions::recordAudio,
                    [this](bool is_granted) {
                        if (!is_granted)
                        {
                            this->systemRequestedQuit();
                        }
                    });
        }

        window = std::make_unique<Window>(getApplicationName());
        juce::ignoreUnused(window);
    }

    void shutdown() override
    {
        window = nullptr;
        juce::ignoreUnused(window);
    }

    [[maybe_unused]] void systemRequestedQuit() override
    {
        quit();
    }

    void anotherInstanceStarted(const juce::String&) override
    {
    }


private:
    class Window : public juce::DocumentWindow
    {
    public:
        explicit Window(juce::String name)
            : DocumentWindow(
                      std::move(name),
                      juce::Desktop::getInstance()
                              .getDefaultLookAndFeel()
                              .findColour(
                                      ResizableWindow::backgroundColourId),
                      DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(
                    new MainComponent(),
                    true);

#if JUCE_IOS || JUCE_ANDROID
            setFullScreen(true);
#else
            setResizable(true, true);
            centreWithSize(getWidth(), getHeight());
#endif

            // this is JUCE code
            setVisible(true);
        }

        void closeButtonPressed() override
        {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Window)
    };

    [[maybe_unused]] std::unique_ptr<Window> window;
};
} // namespace blooper


START_JUCE_APPLICATION(blooper::App)
