#include <blooper/startup/App.hpp>


BLOOPER_NAMESPACE_BEGIN

App::App() = default;

App::~App() = default;


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

    context = std::make_unique<Context>(
            // on success
            [this] {
                this->bodyWindow = std::make_unique<BodyWindow>(
                        *this->context);

                this->bodyWindow->onClose =
                        [this] { this->systemRequestedQuit(); };

                this->bodyWindow->setVisible(true);
            },
            // on fail
            [this] {
                this->systemRequestedQuit();
            });
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
