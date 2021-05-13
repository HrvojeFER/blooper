#ifndef BLOOPER_APP_HPP
#define BLOOPER_APP_HPP


BLOOPER_NAMESPACE_BEGIN

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

        bodyWindow = std::make_unique<BodyWindow>(getApplicationName());
        juce::ignoreUnused(bodyWindow);
    }

    void shutdown() override
    {
        bodyWindow = nullptr;
        juce::ignoreUnused(bodyWindow);
    }

    [[maybe_unused]] void systemRequestedQuit() override
    {
        quit();
    }

    void anotherInstanceStarted(const juce::String&) override
    {
    }


private:
    [[maybe_unused]] std::unique_ptr<BodyWindow> bodyWindow;
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_APP_HPP
