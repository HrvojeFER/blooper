#include <blooper/startup/App.hpp>


BLOOPER_NAMESPACE_BEGIN

// NOLINTNEXTLINE(readability-const-return-type)
const juce::String App::getApplicationName()
{
    return JUCE_APPLICATION_NAME_STRING;
}

// NOLINTNEXTLINE(readability-const-return-type)
const juce::String App::getApplicationVersion()
{
    return JUCE_APPLICATION_VERSION_STRING;
}

bool App::moreThanOneInstanceAllowed()
{
    return true;
}


void App::initialise(const juce::String&)
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
    bodyWindow->onClose = [this] { this->systemRequestedQuit(); };
    bodyWindow->setVisible(true);
}

void App::anotherInstanceStarted(const juce::String&)
{
}


void App::systemRequestedQuit()
{
    quit();
}

void App::shutdown()
{
    bodyWindow = nullptr;
}

BLOOPER_NAMESPACE_END
