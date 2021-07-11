#include <blooper/startup/App.hpp>

#include <blooper/internal/ext/window.hpp>
#include <blooper/internal/utils/callbacks.hpp>

#include <blooper/context/ContextClass.hpp>

#include <blooper/body/BodyWindow.hpp>

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
  return false;
}


void App::initialise(const juce::String& commandLine)
{
  util::requestRuntimePermissions(
      [app = juce::WeakReference<App>(this),
       commandLine](bool granted) {
        if (app.wasObjectDeleted()) return;
        if (!granted) return app->systemRequestedQuit();

        if (juce::RuntimePermissions::isGranted(
                juce::RuntimePermissions::readExternalStorage))
        {
          if (!te::PluginManager::startChildProcessPluginScan(commandLine))
          {
            // TODO
            //  app->systemRequestedQuit();
          }
        }
      });


  ContextOptions options{};

  options.afterLoad =
      ([app = juce::WeakReference<App>(this)] {
        if (app.wasObjectDeleted()) return;

        app->context->loadEngineAsync(JUCE_APPLICATION_NAME_STRING);
      });

  options.afterEngineLoad =
      ([app = juce::WeakReference<App>(this)] {
        if (app.wasObjectDeleted()) return;

        app->context->pickOrLoadProjectAsync();
      });

  options.afterProjectLoad =
      ([app = juce::WeakReference<App>(this)] {
        if (app.wasObjectDeleted()) return;

        BodyWindow::Options bodyOptions{};

        bodyOptions.onClose =
            ([app] {
              if (app.wasObjectDeleted()) return;

              app->systemRequestedQuit();
            });

        app->bodyWindow = std::make_unique<BodyWindow>(
            *app->context,
            app->context->getProjectState().getOrCreateChildWithName(
                BodyWindow::stateId,
                nullptr),
            JuceString(JUCE_APPLICATION_NAME_STRING) +
                JuceString(" ") +
                JuceString(JUCE_APPLICATION_VERSION_STRING),
            move(bodyOptions));

        app->bodyWindow->setVisible(true);
      });

  options.onProjectUnload =
      ([app = juce::WeakReference<App>(this)] {
        if (app.wasObjectDeleted()) return;

        app->bodyWindow.reset();
      });

  options.onEngineUnload =
      ([app = juce::WeakReference<App>(this)] {
        if (app.wasObjectDeleted()) return;

        ext::visitWindows(
            [](juce::TopLevelWindow& window) {
              delete std::addressof(window);
            });
      });

  options.afterEngineUnload =
      ([app = juce::WeakReference<App>(this)] {
        if (app.wasObjectDeleted()) return;

        app->closeAllModalComponentsAsync([app] {
          if (app.wasObjectDeleted()) return;

          app->context->unloadAsync();
        });
      });

  options.afterUnload =
      ([app = juce::WeakReference<App>(this)] {
        if (app.wasObjectDeleted()) return;

        app->quit();
      });

  options.quit =
      ([app = juce::WeakReference<App>(this)] {
        if (app.wasObjectDeleted()) return;

        app->bodyWindow.reset();

        if (app->context->didLoadProject())
        {
          app->context->unloadProjectAsync();
        }
        if (app->context->didLoadEngine())
        {
          app->context->unloadEngineAsync();
        }
        else if (app->context->didLoad())
        {
          app->context->unloadAsync();
        }
      });


  this->context = std::make_unique<Context>(move(options));
  this->context->startAsync();
}

// TODO
void App::anotherInstanceStarted(const juce::String&)
{
}


void App::systemRequestedQuit()
{
  this->context->quit();
}


void App::shutdown()
{
  this->bodyWindow.reset();
  this->context.reset();
}

BLOOPER_NAMESPACE_END
