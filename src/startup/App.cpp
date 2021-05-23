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
  util::requestRuntimePermissions(
      [this](bool granted) {
        if (!granted)
        {
          this->systemRequestedQuit();
        }
        else
        {
          Context::Options options;
          options.onInitSuccess =
              ([this] {
                this->bodyWindow = std::make_unique<BodyWindow>(
                    *this->context);

                this->bodyWindow->onClose =
                    [this] { this->systemRequestedQuit(); };

                this->bodyWindow->setVisible(true);
              });
          options.onInitFailure =
              ([this] {
                this->systemRequestedQuit();
              });

          context = std::make_unique<Context>(options);
        }
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