#include <blooper/context/ContextClass.hpp>

#include <blooper/internal/abstract/const.hpp>
#include <blooper/internal/ext/value_tree.hpp>
#include <blooper/internal/ext/parameter.hpp>
#include <blooper/internal/ext/track.hpp>
#include <blooper/internal/ext/edit.hpp>
#include <blooper/internal/ext/engine.hpp>
#include <blooper/internal/utils/ContextCommands.hpp>

#include <blooper/context/behaviour/behaviour.hpp>
#include <blooper/context/style/LookAndFeel.hpp>

#include <blooper/context/projects/ProjectsMenuWindow.hpp>
#include <blooper/context/projects/ProjectWindow.hpp>
#include <blooper/context/settings/SettingsMenuWindow.hpp>
#include <blooper/context/devices/DevicesMenuWindow.hpp>
#include <blooper/context/plugins/PluginPickerComponent.hpp>

// TODO
// #include <blooper/context/plugins/PluginsMenuWindow.hpp>

#include <blooper/components/help/HelpWindow.hpp>
#include <blooper/components/help/InfoWindow.hpp>
#include <blooper/components/dev/DevWindow.hpp>

BLOOPER_NAMESPACE_BEGIN

Context::Context(ContextOptions options)
    : options(move(options)),

      focusedUndoManager(nullptr),
      focusedSelectionManager(nullptr),

      loaded(false),
      loadedEngine(false),
      loadedProject(false)
{
}

Context::~Context()
{
  ScopedLock scopedLock(this->lock);

  BLOOPER_ASSERT(!this->loadedProject);
  BLOOPER_ASSERT(!this->loadedEngine);
  BLOOPER_ASSERT(!this->loaded);
}


// Expiration

void Context::removeExpiredCommandTargets()
{
  this->commandTargets.removeIf(
      [](const auto& commandTarget) {
        return commandTarget.wasObjectDeleted();
      });
}


// Context

void Context::saveEngineSettings()
{
  {
    auto engineSettingsXml = this->getEngineSettings().createXml();
    this->engineSettingsFile->setValue(
        contextKey,
        engineSettingsXml.get());
  }

  this->engineSettingsFile->save();
}


JuceEditRef Context::getFocusedEdit()
{
  return this->getEditManager().getFocusedEdit();
}

JuceEditRef Context::setFocusedEdit(JuceEditRef edit)
{
  auto focusedEdit = this->getEditManager().setFocusedEdit(*edit);

  this->setFocusedUndoManager(
      std::addressof(focusedEdit->getUndoManager()));

  return std::move(focusedEdit);
}


// PUBLIC

// Special, Async

[[maybe_unused]] void Context::startAsync()
{
  this->loadAsync();
}


[[maybe_unused]] void Context::pickOrLoadProjectAsync()
{
  juce::MessageManager::callAsync(
      [context = juce::WeakReference<Context>(this)] {
        if (context.wasObjectDeleted()) return;
        context->pickOrLoadProject();
      });
}

[[maybe_unused]] void Context::pickProjectAsync()
{
  juce::MessageManager::callAsync(
      [context = juce::WeakReference<Context>(this)] {
        if (context.wasObjectDeleted()) return;
        context->pickProject();
      });
}


[[maybe_unused]] void Context::saveEverythingAsync()
{
  juce::MessageManager::callAsync(
      [context = juce::WeakReference<Context>(this)] {
        if (context.wasObjectDeleted()) return;
        context->saveEverything();
      });
}


[[maybe_unused]] void Context::quitAsync()
{
  juce::MessageManager::callAsync(
      [context = juce::WeakReference<Context>(this)] {
        if (context.wasObjectDeleted()) return;
        context->quit();
      });
}


// Special, Sync

void Context::saveEverything()
{
  ScopedLock scopedLock(this->lock);

  if (this->loadedProject)
  {
    this->log("Saving Context Project.");
    this->saveProjectUnsafe();
    this->log("Saved Context Project.");
  }

  if (this->loadedEngine)
  {
    this->log("Saving Context Engine.");
    this->saveEngineUnsafe();
    this->log("Saved Context Engine.");
  }

  if (this->loaded)
  {
    this->log("Saving Context.");
    this->saveUnsafe();
    this->log("Saved Context.");
  }
}


[[maybe_unused]] void Context::pickOrLoadProject()
{
  ScopedLock scopedLock(this->lock);

  BLOOPER_ASSERT(this->loaded);
  BLOOPER_ASSERT(this->loadedEngine);
  if (!this->loaded ||
      !this->loadedEngine) return;

  if (this->openProjectFilePath->isEmpty())
  {
    this->log("Picking project.");
    this->pickProjectAsync();
  }
  else
  {
    this->log("Already picked a project.");
    this->reloadProjectAsync();
  }
}

void Context::pickProject()
{
  ProjectsMenuWindow::Options windowOptions;

  windowOptions.onOpen =
      ([context = juce::WeakReference<Context>(this)](
           JuceProjectRef projectRef) {
        if (context.wasObjectDeleted()) return;

        context->reloadProjectAsync(move(projectRef));
      });

  windowOptions.onCancel =
      ([context = juce::WeakReference<Context>(this)] {
        if (context.wasObjectDeleted()) return;

        {
          ScopedLock scopedLock(context->lock);
          if (!context->loadedProject) context->quitAsync();
        }
      });

  showProjectsMenu(*this, move(windowOptions));
}


void Context::quit()
{
  ScopedLock scopedLock(this->lock);

  this->log("Quitting.");
  this->options.quit();
}


// Safe, Async

[[maybe_unused]] void Context::loadAsync()
{
  juce::MessageManager::callAsync(
      [context = juce::WeakReference<Context>(this)] {
        if (context.wasObjectDeleted()) return;
        context->load();
      });
}

[[maybe_unused]] void Context::saveAsync()
{
  juce::MessageManager::callAsync(
      [context = juce::WeakReference<Context>(this)] {
        if (context.wasObjectDeleted()) return;
        context->save();
      });
}

[[maybe_unused]] void Context::unloadAsync()
{
  juce::MessageManager::callAsync(
      [context = juce::WeakReference<Context>(this)] {
        if (context.wasObjectDeleted()) return;
        context->unload();
      });
}


[[maybe_unused]] void Context::loadEngineAsync(JuceString name)
{
  juce::MessageManager::callAsync(
      [context = juce::WeakReference<Context>(this),
       name = move(name)] {
        if (context.wasObjectDeleted()) return;
        context->loadEngine(move(name));
      });
}

[[maybe_unused]] void Context::saveEngineAsync()
{
  juce::MessageManager::callAsync(
      [context = juce::WeakReference<Context>(this)] {
        if (context.wasObjectDeleted()) return;
        context->saveEngine();
      });
}

[[maybe_unused]] void Context::unloadEngineAsync()
{
  juce::MessageManager::callAsync(
      [context = juce::WeakReference<Context>(this)] {
        if (context.wasObjectDeleted()) return;
        context->unloadEngine();
      });
}


[[maybe_unused]] void Context::loadProjectAsync(JuceProjectRef ref)
{
  juce::MessageManager::callAsync(
      [context = juce::WeakReference<Context>(this),
       ref = move(ref)] {
        if (context.wasObjectDeleted()) return;
        context->loadProject(ref);
      });
}

[[maybe_unused]] void Context::saveProjectAsync()
{
  juce::MessageManager::callAsync(
      [context = juce::WeakReference<Context>(this)] {
        if (context.wasObjectDeleted()) return;
        context->saveProject();
      });
}

[[maybe_unused]] void Context::unloadProjectAsync()
{
  juce::MessageManager::callAsync(
      [context = juce::WeakReference<Context>(this)] {
        if (context.wasObjectDeleted()) return;
        context->unloadProject();
      });
}


// Safe, Sync

void Context::load()
{
  ScopedLock scopedLock(this->lock);

  BLOOPER_ASSERT(!this->loaded);
  BLOOPER_ASSERT(!this->loadedEngine);
  BLOOPER_ASSERT(!this->loadedProject);
  if (this->loaded ||
      this->loadedEngine ||
      this->loadedProject) return;

  this->options.onLoad();

  this->loadUnsafe();
  this->loaded = true;

  this->log("Loaded Context.");

  this->options.afterLoad();
}

void Context::unload()
{
  ScopedLock scopedLock(this->lock);

  BLOOPER_ASSERT(this->loaded);
  BLOOPER_ASSERT(!this->loadedEngine);
  BLOOPER_ASSERT(!this->loadedProject);
  if (!this->loaded ||
      this->loadedEngine ||
      this->loadedProject) return;

  this->options.onUnload();

  this->log("Unloading Context.");

  this->unloadUnsafe();
  this->loaded = false;

  this->options.afterUnload();
}

void Context::save()
{
  ScopedLock scopedLock(this->lock);

  BLOOPER_ASSERT(this->loaded);
  if (!this->loaded) return;

  this->log("Saving Context.");

  this->saveUnsafe();

  this->log("Saved Context.");
}


void Context::loadEngine(JuceString name)
{
  ScopedLock scopedLock(this->lock);

  BLOOPER_ASSERT(this->loaded);
  BLOOPER_ASSERT(!this->loadedEngine);
  BLOOPER_ASSERT(!this->loadedProject);
  if (!this->loaded ||
      this->loadedEngine ||
      this->loadedProject) return;

  this->options.onEngineLoad();

  this->log("Loading Context Engine.");

  this->loadEngineUnsafe(move(name));
  this->loadedEngine = true;

  this->log("Loaded Context Engine.");

  this->options.afterEngineLoad();
}

void Context::unloadEngine()
{
  ScopedLock scopedLock(this->lock);

  BLOOPER_ASSERT(this->loaded);
  BLOOPER_ASSERT(this->loadedEngine);
  BLOOPER_ASSERT(!this->loadedProject);
  if (!this->loaded ||
      !this->loadedEngine ||
      this->loadedProject) return;

  this->options.onEngineUnload();

  this->log("Unloading Context Engine.");

  this->unloadEngineUnsafe();
  this->loadedEngine = false;

  this->log("Unloaded Context Engine.");

  this->options.afterEngineUnload();
}

void Context::saveEngine()
{
  ScopedLock scopedLock(this->lock);

  BLOOPER_ASSERT(this->loadedEngine);
  if (!this->loadedEngine) return;

  this->log("Saving Context Engine.");

  this->saveEngineUnsafe();

  this->log("Saved Context Engine.");
}


void Context::loadProject(JuceProjectRef ref)
{
  ScopedLock scopedLock(this->lock);

  BLOOPER_ASSERT(this->loaded);
  BLOOPER_ASSERT(this->loadedEngine);
  BLOOPER_ASSERT(!this->loadedProject);
  if (!this->loaded ||
      !this->loadedEngine ||
      this->loadedProject) return;

  this->options.onProjectLoad();

  this->log("Loading Context Project.");

  this->loadProjectUnsafe(move(ref));
  this->loadedProject = true;

  this->log("Loaded Context Project.");

  this->options.afterProjectLoad();
}

void Context::unloadProject()
{
  ScopedLock scopedLock(this->lock);

  BLOOPER_ASSERT(this->loaded);
  BLOOPER_ASSERT(this->loadedEngine);
  BLOOPER_ASSERT(this->loadedProject);
  if (!this->loaded ||
      !this->loadedEngine ||
      !this->loadedProject) return;

  this->options.onProjectUnload();

  this->log("Unloading Context Project.");

  this->unloadProjectUnsafe();
  this->loadedProject = false;

  this->log("Unloaded Context Project.");

  this->options.afterProjectUnload();
}

void Context::saveProject()
{
  ScopedLock scopedLock(this->lock);

  BLOOPER_ASSERT(this->loadedProject);
  if (!this->loadedProject) return;

  this->log("Saving Context Project.");

  this->saveProjectUnsafe();

  this->log("Saved Context Project.");
}


// Unsafe, sync

[[maybe_unused]] bool Context::didLoad() const
{
  return this->loaded;
}

[[maybe_unused]] bool Context::didLoadEngine() const
{
  return this->loadedEngine;
}

[[maybe_unused]] bool Context::didLoadProject() const
{
  return this->loadedProject;
}


// PRIVATE

// Special, Async

[[maybe_unused]] void Context::reloadProjectAsync(JuceProjectRef ref)
{
  juce::MessageManager::callAsync(
      [context = juce::WeakReference<Context>(this),
       ref = move(ref)] {
        if (context.wasObjectDeleted()) return;
        context->reloadProject(ref);
      });
}

[[maybe_unused]] void Context::reloadProjectAsync()
{
  juce::MessageManager::callAsync(
      [context = juce::WeakReference<Context>(this)] {
        if (context.wasObjectDeleted()) return;
        context->reloadProject();
      });
}


// Special, Sync

void Context::reloadProject(JuceProjectRef ref)
{
  ScopedLock scopedLock(this->lock);

  BLOOPER_ASSERT(this->loaded);
  BLOOPER_ASSERT(this->loadedEngine);
  if (!this->loaded ||
      !this->loadedEngine) return;


  if (this->loadedProject)
  {
    this->options.onProjectUnload();

    this->log("Unloading Context Project.");

    this->unloadProjectUnsafe();
    this->loadedProject = false;

    this->log("Unloaded Context Project.");

    this->options.afterProjectUnload();
  }


  this->options.onProjectLoad();

  this->log("Loading Context Project.");

  this->loadProjectUnsafe(move(ref));
  this->loadedProject = true;

  this->log("Loaded Context Project.");

  this->options.afterProjectLoad();
}

void Context::reloadProject()
{
  ScopedLock scopedLock(this->lock);

  BLOOPER_ASSERT(this->loaded);
  BLOOPER_ASSERT(this->loadedEngine);
  if (!this->loaded ||
      !this->loadedEngine) return;

  if (this->loadedProject)
  {
    this->options.onProjectUnload();

    this->log("Unloading Context Project.");

    this->unloadProjectUnsafe();
    this->loadedProject = false;

    this->log("Unloaded Context Project.");

    this->options.afterProjectUnload();
  }


  this->options.onProjectLoad();

  this->log("Loading Context Project.");

  auto& projectManager = this->getEngine().getProjectManager();
  auto  projectRef =
      projectManager.findProjectWithFile(
          projectManager.folders,
          JuceFile{this->openProjectFilePath});
  BLOOPER_ASSERT(projectRef);
  this->loadProjectUnsafe(move(projectRef));
  this->loadedProject = true;

  this->log("Loaded Context Project.");

  this->options.afterProjectLoad();
}


// Unlocked, Sync

void Context::loadUnsafe()
{
  this->rootDir =
      ext::ensureExistingDirectory(
          JuceFile::getSpecialLocation(
              rootDirSpecialLocation)
              .getChildFile(rootDirName));

  this->settingsFile =
      ext::ensureValidStateFile(
          this->getRootDir().getChildFile(
              settingsFileName));
  this->settings =
      ext::ensureValidState(
          *this->settingsFile,
          contextKey,
          Context::stateId);

  this->stateFile =
      ext::ensureValidStateFile(
          this->getRootDir().getChildFile(
              stateFileName));
  this->state =
      ext::ensureValidState(
          *this->stateFile,
          contextKey,
          Context::stateId);


  this->logDir =
      ext::ensureExistingDirectory(
          this->getRootDir().getChildFile(logDirName));
  this->logFile =
      ext::ensureExistingFile(
          this->logDir->getChildFile(
              JuceFile::createLegalFileName(
                  logFileNamePrefix +
                  juce::Time::getCurrentTime()
                      .toString(
                          true,
                          true,
                          true,
                          true) +
                  logFileNameSuffix)
                  .replaceCharacter(
                      ' ',
                      '_')));
  this->logger =
      std::make_unique<juce::FileLogger>(
          *this->logFile,
          JuceString{"This is a blooper log file! "
                     "If you are reading this, chances are that you are "
                     "having problems with blooper so I encourage you "
                     "not to edit this file and send it to the "
                     "development team if necessary."});
  JuceLogger::setCurrentLogger(this->logger.get());

  this->commandManager = std::make_unique<JuceCommandManager>();
  this->commandTargets.clear();
  this->commandManager->registerAllCommandsForTarget(this);
  this->commandManager->setFirstCommandTarget(this);
  this->lastCommandId.referTo(
      this->getState(),
      id::lastCommandId,
      nullptr,
      CommandId::none);

  this->undoManager =
      std::make_unique<JuceUndoManager>();
  this->focusedUndoManager = this->undoManager.get();

  this->selectionManager =
      std::make_unique<JuceSelectionManager>(
          this->getEngine());
  this->focusedSelectionManager = this->selectionManager.get();
}

void Context::unloadUnsafe()
{
  State invalidState{};

  {
    auto stateXml = this->getState().createXml();
    this->stateFile->setValue(
        contextKey,
        stateXml.get());
  }

  this->stateFile->save();


  this->selectionManager.reset();

  this->undoManager.reset();

  this->lastCommandId.referTo(
      invalidState,
      {},
      nullptr);
  this->commandTargets.clear();
  this->commandManager->setFirstCommandTarget(nullptr);
  this->commandManager.reset();

  JuceLogger::setCurrentLogger(nullptr);
  this->logger.reset();
  this->logFile.reset();
  this->logDir.reset();


  this->state = {};
  this->stateFile.reset();

  this->settings = {};
  this->settingsFile.reset();

  this->rootDir.reset();
}

void Context::saveUnsafe()
{
  {
    auto settingsXml = this->getSettings().createXml();
    this->settingsFile->setValue(
        contextKey,
        settingsXml.get());
  }
  {
    auto stateXml = this->getState().createXml();
    this->stateFile->setValue(
        contextKey,
        stateXml.get());
  }

  this->settingsFile->save();
  this->stateFile->save();
}


void Context::loadEngineUnsafe(JuceString name)
{
  this->lookAndFeel = std::make_unique<LookAndFeel>(*this);
  JuceLookAndFeel::setDefaultLookAndFeel(
      this->lookAndFeel.get());

  this->assetManager = std::make_unique<AssetManager>(*this);

  this->tooltipWindow = std::make_unique<juce::TooltipWindow>();
  this->tooltipWindow->setLookAndFeel(this->lookAndFeel.get());


  this->engineSettingsFile =
      ext::ensureValidStateFile(
          this->getRootDir().getChildFile(
              engineSettingsFileName));
  this->engineSettings =
      ext::ensureValidState(
          *this->engineSettingsFile,
          contextKey,
          Context::stateId);

  this->projectsDir =
      ext::ensureExistingDirectory(
          this->getRootDir().getChildFile(
              projectsDirName));


  this->engine =
      std::make_unique<JuceEngine>(
          std::make_unique<PropertyStorage>(move(name), *this),
          std::make_unique<UIBehaviour>(*this),
          std::make_unique<EngineBehaviour>(*this));
  this->getEngine().getProjectManager().loadList();


  this->monitored.referTo(
      this->getState(),
      id::monitored,
      this->undoManager.get(),
      true);

  this->openProjectFilePath.referTo(
      this->getState(),
      id::openProject,
      std::addressof(this->getUndoManager()),
      "");
}

void Context::unloadEngineUnsafe()
{
  State invalidState{};

  this->openProjectFilePath.referTo(
      invalidState,
      {},
      nullptr);

  this->monitored.referTo(
      invalidState,
      {},
      nullptr);


  this->engine->getTemporaryFileManager()
      .getTempDirectory()
      .deleteRecursively();
  this->engine->getProjectManager().clearProjects();
  this->engine.reset();


  this->projectsDir.reset();

  this->engineSettings = {};
  this->engineSettingsFile.reset();


  this->tooltipWindow.reset();

  this->assetManager.reset();

  JuceLookAndFeel::setDefaultLookAndFeel(nullptr);
  this->lookAndFeel.reset();
}

void Context::saveEngineUnsafe()
{
  this->engine->getDeviceManager().saveSettings();

  {
    auto engineSettingsXml = this->getEngineSettings().createXml();
    this->engineSettingsFile->setValue(
        contextKey,
        engineSettingsXml.get());
  }

  this->engineSettingsFile->save();
}


void Context::loadProjectUnsafe(JuceProjectRef ref)
{
  this->project = move(ref);

  this->openProjectFilePath =
      this->getProject().getProjectFile().getFullPathName();


  this->projectSettingsFile =
      ext::ensureValidStateFile(
          this->getProject()
              .getProjectFile()
              .getParentDirectory()
              .getChildFile(
                  settingsFileName));

  const auto& normalSettingsFile =
      this->projectSettingsFile->getFile();

  if (auto settingsItem =
          this->getProject()
              .getProjectItemForFile(normalSettingsFile))
  {
    this->projectSettingsItem = move(settingsItem);
  }
  else
  {
    this->projectSettingsItem =
        this->getProject().createNewItem(
            normalSettingsFile,
            "settings",
            "settings",
            "Project settings.",
            te::ProjectItem::Category::none,
            true);
  }

  this->projectSettings =
      ext::ensureValidState(
          *this->projectSettingsFile,
          contextKey,
          Context::stateId);


  this->projectStateFile =
      ext::ensureValidStateFile(
          this->getProject()
              .getProjectFile()
              .getParentDirectory()
              .getChildFile(
                  stateFileName));

  const auto& normalStateFile = this->projectStateFile->getFile();

  if (auto stateItem =
          this->getProject()
              .getProjectItemForFile(normalStateFile))
  {
    this->projectStateItem = move(stateItem);
  }
  else
  {
    this->projectStateItem =
        this->getProject().createNewItem(
            normalStateFile,
            "state",
            "state",
            "Project state.",
            te::ProjectItem::Category::none,
            true);
  }

  this->projectState =
      ext::ensureValidState(
          *this->projectStateFile,
          contextKey,
          Context::stateId);


  // TODO: manage this a bit more elegantly
  // before edit manager because it stops the master Edit
  // also, don't touch this too much, because I tried and it gives me access
  // violations for doing it in one loop - maybe some lock somewhere?
  auto& deviceManager = this->getEngine().getDeviceManager();
  for (int i = 0; i < deviceManager.getNumWaveInDevices(); i++)
  {
    if (auto input = deviceManager.getWaveInDevice(i))
    {
      input->setStereoPair(false);
    }
  }
  for (int i = 0; i < deviceManager.getNumWaveInDevices(); i++)
  {
    if (auto input = deviceManager.getWaveInDevice(i))
    {
      input->setEndToEnd(this->monitored);
      input->setEnabled(true);
    }
  }


  this->editManager =
      std::make_unique<EditManager>(
          *this,
          this->projectState.getOrCreateChildWithName(
              EditManager::stateId,
              nullptr));

  this->synchronizer =
      std::make_unique<Synchronizer>(
          *this,
          this->projectState.getOrCreateChildWithName(
              Synchronizer::stateId,
              nullptr));
}

void Context::unloadProjectUnsafe()
{
  {
    auto stateXml = this->getProjectState().createXml();
    this->projectStateFile->setValue(
        contextKey,
        stateXml.get());
  }

  this->projectStateFile->save();


  this->synchronizer.reset();

  this->editManager.reset();


  this->projectState = {};
  this->projectStateFile.reset();

  this->projectSettings = {};
  this->projectSettingsFile.reset();


  this->project.reset();
}

void Context::saveProjectUnsafe()
{
  {
    auto settingsXml = this->getProjectSettings().createXml();
    this->projectSettingsFile->setValue(
        contextKey,
        settingsXml.get());
  }
  {
    auto stateXml = this->getProjectState().createXml();
    this->projectStateFile->setValue(
        contextKey,
        stateXml.get());
  }

  this->projectSettingsFile->save();
  this->projectStateFile->save();

  this->project->save();
}


// CommandTarget

juce::ApplicationCommandTarget* Context::getNextCommandTarget()
{
  return nullptr;
}

void Context::getAllCommands(juce::Array<JuceCommandId>& commands)
{
  fillCommands(
      commands,

      // App

      CommandId::quit,

      CommandId::saveProject,
      CommandId::saveEdit,
      CommandId::saveAll,
      CommandId::saveAndQuit,
      CommandId::saveAs,

      CommandId::openSettings,
      CommandId::openProjectSettings,
      CommandId::openDeviceManager,

      CommandId::openProject,

      CommandId::openHelp,
      CommandId::openInfo,
      CommandId::openDev,


      // Engine

      CommandId::toggleMonitoring,


      // Edit

      CommandId::del,
      CommandId::cut,
      CommandId::copy,
      CommandId::paste,

      CommandId::undo,
      CommandId::redo,

      CommandId::addTrack,
      CommandId::addPlugin,

      CommandId::deselectAll,


      // Transport

      CommandId::togglePlaying,
      CommandId::toggleRecording,


      // Track

      CommandId::toggleMuted,
      CommandId::toggleSoloed,
      CommandId::toggleArmed,

      CommandId::cycleTrackMode,
      CommandId::cycleTrackInterval,

      CommandId::clearTrack,

      // Parameter

      CommandId::nudgeUp,
      CommandId::nudgeDown);


  for (const auto& target : this->commandTargets)
    if (!target.wasObjectDeleted())
      target->getAllCommands(commands);
}

void Context::getCommandInfo(
    JuceCommandId    commandID,
    JuceCommandInfo& result)
{
  fillCommandInfo(
      result,
      commandID);

  result.setActive(true);

  for (const auto& target : this->commandTargets)
    if (!target.wasObjectDeleted())
      if (auto performer = target->getTargetForCommand(commandID))
        performer->getCommandInfo(commandID, result);
}

bool Context::perform(const JuceCommand& command)
{
  switch (command.commandID)
  {
      // App

    case CommandId::quit:
      this->quitAsync();
      return true;


    case CommandId::saveProject:
      this->saveProjectAsync();
      return true;

    case CommandId::saveEdit:
      if (auto edit = this->getFocusedEdit())
        te::EditFileOperations{*edit}
            .save(
                true,
                false,
                false);

      return true;

    case CommandId::saveAll:
      this->saveEverythingAsync();
      return true;

    case CommandId::saveAndQuit:
      this->saveEverythingAsync();
      this->quitAsync();
      return true;

      // TODO
    case CommandId::saveAs:
      this->saveProjectAsync();
      return true;


    case CommandId::openSettings:
      {
        SettingsMenuWindow::Options settingsOptions{};
        showSettingsMenu(
            *this,
            move(settingsOptions));

        return true;
      }

    case CommandId::openProjectSettings:
      {
        ProjectWindow::Options projectOptions{};
        showProject(
            *this,
            this->getProjectRef(),
            move(projectOptions));

        return true;
      }

    case CommandId::openDeviceManager:
      {
        DevicesMenuWindow::Options devicesOptions{};
        showDevicesMenu(
            *this,
            move(devicesOptions));

        return true;
      }


    case CommandId::openProject:
      this->pickProjectAsync();
      return true;


    case CommandId::openHelp:
      {
        HelpWindow::Options helpOptions{};
        showHelp(*this, move(helpOptions));
        return true;
      }

    case CommandId::openInfo:
      {
        InfoWindow::Options infoOptions{};
        showInfo(*this, move(infoOptions));
        return true;
      }

    case CommandId::openDev:
      {
        DevWindow::Options devOptions{};
        showDev(*this, move(devOptions));
        return true;
      }


      // Engine

    case CommandId::toggleMonitoring:
      ext::toggleMonitoring(this->getEngine());

      return true;


      // Edit

    case CommandId::del:
      if (auto focusedSelection = this->getFocusedSelectionManager())
        focusedSelection->deleteSelected();

      return true;

    case CommandId::cut:
      if (auto focusedSelection = this->getFocusedSelectionManager())
        focusedSelection->cutSelected();

      return true;

    case CommandId::copy:
      if (auto focusedSelection = this->getFocusedSelectionManager())
        focusedSelection->copySelected();

      return true;

    case CommandId::paste:
      if (auto focusedSelection = this->getFocusedSelectionManager())
        focusedSelection->pasteSelected();

      return true;


      // TODO: better

    case CommandId::addEdit:
      this->getEditManager().add();

      return true;

    case CommandId::addTrack:
      if (auto edit = this->getFocusedEdit())
        ext::addAudioTrack(
            *edit,
            this->getFocusedSelectionManager());

      return true;

    case CommandId::addPlugin:
      if (auto focusedSelection = this->getFocusedSelectionManager())
      {
        if (focusedSelection->getFirstItemOfType<te::Track>())
        {
          auto plugin = pickPlugin(*this);

          for (auto track : focusedSelection->getItemsOfType<te::Track>())
            ext::addPlugin(*track, plugin, focusedSelection);
        }
      }
      return true;


    case CommandId::undo:
      if (auto focusedUndo = this->getFocusedUndoManager())
        focusedUndo->undo();

      return true;

    case CommandId::redo:
      if (auto focusedUndo = this->getFocusedUndoManager())
        focusedUndo->redo();

      return true;


    case CommandId::deselectAll:
      if (auto focusedSelection = this->getFocusedSelectionManager())
        focusedSelection->deselectAll();

      return true;


      // Transport

    case CommandId::togglePlaying:
      if (auto edit = this->getFocusedEdit())
        ext::togglePlaying(
            *edit,
            std::addressof(
                this->getEditManager()
                    .getMasterEdit()));

      return true;

    case CommandId::toggleRecording:
      if (auto edit = this->getFocusedEdit())
        ext::toggleRecording(
            *edit,
            std::addressof(
                this->getEditManager()
                    .getMasterEdit()));

      return true;


      // Track

    case CommandId::toggleMuted:
      if (auto focusedSelection = this->getFocusedSelectionManager())
        for (auto track : focusedSelection->getItemsOfType<te::Track>())
          ext::toggleMuted(*track);
      return true;

    case CommandId::toggleSoloed:
      if (auto focusedSelection = this->getFocusedSelectionManager())
        for (auto track : focusedSelection->getItemsOfType<te::Track>())
          ext::toggleSoloed(*track);
      return true;

    case CommandId::toggleArmed:
      if (auto focusedSelection = this->getFocusedSelectionManager())
        for (auto track : focusedSelection->getItemsOfType<te::Track>())
          ext::toggleArmed(*track);

      return true;


    case CommandId::cycleTrackMode:
      if (auto focusedSelection = this->getFocusedSelectionManager())
        for (auto track : focusedSelection->getItemsOfType<te::Track>())
          ext::cycleTrackMode(*track);

      return true;

    case CommandId::cycleTrackInterval:
      if (auto focusedSelection = this->getFocusedSelectionManager())
        for (auto track : focusedSelection->getItemsOfType<te::Track>())
          ext::cycleTrackInterval(*track);

      return true;


    case CommandId::clearTrack:
      if (auto focusedSelection = this->getFocusedSelectionManager())
        for (auto track : focusedSelection->getItemsOfType<te::Track>())
          ext::clear(*track);

      return true;


      // Parameter

    case CommandId::nudgeUp:
      if (auto focusedSelection = this->getFocusedSelectionManager())
        for (auto parameter :
             focusedSelection->getItemsOfType<te::AutomatableParameter>())
          ext::nudgeParameterUp(*parameter);

      return true;

    case CommandId::nudgeDown:
      if (auto focusedSelection = this->getFocusedSelectionManager())
        for (auto parameter :
             focusedSelection->getItemsOfType<te::AutomatableParameter>())
          ext::nudgeParameterDown(*parameter);

      return true;


    default:
      break;
  }


  bool actionPerformed = false;

  for (const auto& target : this->commandTargets)
  {
    if (!target.wasObjectDeleted())
    {
      if (auto performer = target->getTargetForCommand(command.commandID))
      {
        if (performer->perform(command))
        {
          actionPerformed = true;
          break;
        }
      }
    }
  }

  this->removeExpiredCommandTargets();


  return actionPerformed;
}


// ApplicationCommandManagerListener

void Context::applicationCommandInvoked(
    JuceCommand const& command)
{
  this->lastCommandId = command.commandID;
}

void Context::applicationCommandListChanged()
{
}

BLOOPER_NAMESPACE_END
