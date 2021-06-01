#include <blooper/blooper.hpp>

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
      [app = juce::WeakReference<App>(this)](bool granted) {
        if (!granted && !app.wasObjectDeleted())
          return app->systemRequestedQuit();
      });

  Context::Options options{};
  options.onInitSuccess =
      ([app = juce::WeakReference<App>(this)] {
        BodyWindow::Options bodyOptions{};

        bodyOptions.onClose =
            ([app] {
              if (!app.wasObjectDeleted())
                app->systemRequestedQuit();
            });

        if (!app.wasObjectDeleted())
        {
          app->bodyWindow = std::make_unique<BodyWindow>(
              *app->context,
              app->context->getState().getOrCreateChildWithName(
                  BodyWindow::stateId,
                  nullptr),
              JuceString(JUCE_APPLICATION_NAME_STRING) +
                  JuceString("_") +
                  JuceString(JUCE_APPLICATION_VERSION_STRING),
              move(bodyOptions));

          app->bodyWindow->setVisible(true);
        }
      });

  options.onInitFailure =
      ([app = juce::WeakReference<App>(this)] {
        if (!app.wasObjectDeleted())
          app->systemRequestedQuit();
      });

  this->context = std::make_unique<Context>(
      JUCE_APPLICATION_NAME_STRING,
      options);
}

void App::anotherInstanceStarted(const juce::String&)
{
}


void App::systemRequestedQuit()
{
  if (juce::ModalComponentManager::getInstance()->cancelAllModalComponents())
  {
    juce::Timer::callAfterDelay(
        App::quitRetryIntervalMilliseconds,
        [app = juce::WeakReference<App>(this)]() {
          if (!app.wasObjectDeleted()) app->systemRequestedQuit();
        });
  }
  else
  {
    quit();
  }
}

void App::shutdown()
{
  bodyWindow = nullptr;
  BLOOPER_UNUSED(bodyWindow);

  context = nullptr;
  BLOOPER_UNUSED(context);
}

BLOOPER_NAMESPACE_END
