#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

Context::Options::Options()
    : quit([] {}),

      onLoad([] {}),
      afterLoad([] {}),
      onUnload([] {}),
      afterUnload([] {}),

      onEngineLoad([] {}),
      afterEngineLoad([] {}),
      onEngineUnload([] {}),
      afterEngineUnload([] {}),

      onProjectLoad([] {}),
      afterProjectLoad([] {}),
      onProjectUnload([] {}),
      afterProjectUnload([] {})
{
}

Context::Context(Options options)
    : options(move(options)),

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


// Context

void Context::saveEngineSettings()
{
  {
    auto engineSettingsXml = this->getEngineSettings().createXml();
    this->engineSettingsFile->setValue(
        Context::fileKey,
        engineSettingsXml.get());
  }

  this->engineSettingsFile->save();
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
              Context::rootDirSpecialLocation)
              .getChildFile(Context::rootDirName));

  this->projectsDir =
      ext::ensureExistingDirectory(
          this->getRootDir().getChildFile(
              Context::projectsDirName));

  this->engineSettingsFile =
      ext::ensureValidStateFile(
          this->getRootDir().getChildFile(
              Context::engineSettingsFileName));
  this->engineSettings =
      ext::ensureValidState(
          *this->engineSettingsFile,
          Context::fileKey,
          Context::stateId);

  this->settingsFile =
      ext::ensureValidStateFile(
          this->getRootDir().getChildFile(
              Context::settingsFileName));
  this->settings =
      ext::ensureValidState(
          *this->settingsFile,
          Context::fileKey,
          Context::stateId);

  this->stateFile =
      ext::ensureValidStateFile(
          this->getRootDir().getChildFile(
              Context::stateFileName));
  this->state =
      ext::ensureValidState(
          *this->stateFile,
          Context::fileKey,
          Context::stateId);

  this->openProjectFilePath.referTo(
      this->getState(),
      id::openProject,
      std::addressof(this->getUndoManager()),
      "");


  this->logDir =
      ext::ensureExistingDirectory(
          this->getRootDir().getChildFile(Context::logDirName));
  this->logFile =
      ext::ensureExistingFile(
          this->logDir->getChildFile(
              JuceFile::createLegalFileName(
                  Context::logFileNamePrefix +
                  juce::Time::getCurrentTime()
                      .toString(
                          true,
                          true,
                          true,
                          true) +
                  Context::logFileNameSuffix)
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

  this->assetManager = std::make_unique<AssetManager>(*this);

  this->undoManager = std::make_unique<JuceUndoManager>();

  this->commandManager = std::make_unique<JuceCommandManager>();
  this->commandTargets.clear();
  this->commandManager->registerAllCommandsForTarget(this);
  this->commandManager->setFirstCommandTarget(this);

  this->lookAndFeel = std::make_unique<LookAndFeel>(*this);
  JuceLookAndFeel::setDefaultLookAndFeel(
      this->lookAndFeel.get());
}

void Context::unloadUnsafe()
{
  JuceLookAndFeel::setDefaultLookAndFeel(nullptr);
  this->lookAndFeel.reset();

  this->commandTargets.clear();
  this->commandManager->setFirstCommandTarget(nullptr);
  this->commandManager.reset();

  this->undoManager.reset();

  this->assetManager.reset();

  JuceLogger::setCurrentLogger(nullptr);
  this->logger.reset();
  this->logFile.reset();
  this->logDir.reset();


  this->state = {};
  this->stateFile.reset();

  this->openProjectFilePath.referTo(
      // it has to be an lvalue for some reason
      // even though it is used for copying..
      this->state,
      {},
      nullptr);

  this->settings = {};
  this->settingsFile.reset();

  this->engineSettings = {};
  this->engineSettingsFile.reset();

  this->projectsDir.reset();

  this->rootDir.reset();
}

void Context::saveUnsafe()
{
  // TODO?
}


void Context::loadEngineUnsafe(JuceString name)
{
  this->tooltipWindow = std::make_unique<juce::TooltipWindow>();
  this->tooltipWindow->setLookAndFeel(this->lookAndFeel.get());

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

  this->selectionManager =
      std::make_unique<JuceSelectionManager>(
          this->getEngine());
}

void Context::unloadEngineUnsafe()
{
  {
    auto stateXml = this->getState().createXml();
    this->stateFile->setValue(
        Context::fileKey,
        stateXml.get());
  }

  this->stateFile->save();


  this->selectionManager.reset();

  this->engine->getTemporaryFileManager()
      .getTempDirectory()
      .deleteRecursively();
  this->engine->getProjectManager().clearProjects();
  this->engine.reset();

  this->tooltipWindow.reset();
}

void Context::saveEngineUnsafe()
{
  this->engine->getProjectManager().saveList();

  {
    auto engineSettingsXml = this->getEngineSettings().createXml();
    this->engineSettingsFile->setValue(
        Context::fileKey,
        engineSettingsXml.get());
  }
  {
    auto settingsXml = this->getSettings().createXml();
    this->settingsFile->setValue(
        Context::fileKey,
        settingsXml.get());
  }
  {
    auto stateXml = this->getState().createXml();
    this->stateFile->setValue(
        Context::fileKey,
        stateXml.get());
  }

  this->engineSettingsFile->save();
  this->settingsFile->save();
  this->stateFile->save();
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
                  Context::settingsFileName));

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
          Context::fileKey,
          Context::stateId);


  this->projectStateFile =
      ext::ensureValidStateFile(
          this->getProject()
              .getProjectFile()
              .getParentDirectory()
              .getChildFile(
                  Context::stateFileName));

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
          Context::fileKey,
          Context::stateId);


  this->synchronizer =
      std::make_unique<Synchronizer>(
          *this,
          this->projectState.getOrCreateChildWithName(
              Synchronizer::stateId,
              nullptr));

  this->editManager = std::make_unique<EditManager>(*this);


  // TODO: manage this a bit more elegantly
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
}

void Context::unloadProjectUnsafe()
{
  {
    auto stateXml = this->getProjectState().createXml();
    this->projectStateFile->setValue(
        Context::fileKey,
        stateXml.get());
  }

  this->projectStateFile->save();


  this->editManager.reset();

  this->synchronizer.reset();

  this->projectState = {};
  this->projectStateFile.reset();

  this->projectSettings = {};
  this->projectSettingsFile.reset();


  this->openProjectFilePath = "";

  this->project.reset();
}

void Context::saveProjectUnsafe()
{
  {
    auto settingsXml = this->getProjectSettings().createXml();
    this->projectSettingsFile->setValue(
        Context::fileKey,
        settingsXml.get());
  }
  {
    auto stateXml = this->getProjectState().createXml();
    this->projectStateFile->setValue(
        Context::fileKey,
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

      CommandId::save,
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

      CommandId::play,
      CommandId::pause,
      CommandId::stop,
      CommandId::record,

      CommandId::toggleMonitoring,


      // Track

      CommandId::muteTrack,
      CommandId::soloTrack,
      CommandId::armTrack,

      CommandId::clearTrack,

      // Parameter

      CommandId::nudgeUp,
      CommandId::nudgeDown);


  for (const auto& target : this->commandTargets)
    if (!target.wasObjectDeleted())
      target->getAllCommands(commands);
}

void Context::getCommandInfo(JuceCommandId commandID, JuceCommandInfo& result)
{
  fillCommandInfo(
      result,
      commandID);

  result.setActive(true);

  for (const auto& target : this->commandTargets)
    if (!target.wasObjectDeleted())
      target->getCommandInfo(commandID, result);
}

bool Context::perform(const JuceCommand& command)
{
  switch (command.commandID)
  {
      // App

    case CommandId::quit:
      this->quitAsync();
      return true;


    case CommandId::save:
      this->saveProjectAsync();
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


      // Edit

    case CommandId::del:
      if (auto focusedSelection =
              this->getEngine()
                  .getUIBehaviour()
                  .getCurrentlyFocusedSelectionManager())
      {
        focusedSelection->deleteSelected();
      }
      else
      {
        this->getSelectionManager().deleteSelected();
      }
      return true;

    case CommandId::cut:
      if (auto focusedSelection =
              this->getEngine()
                  .getUIBehaviour()
                  .getCurrentlyFocusedSelectionManager())
      {
        focusedSelection->cutSelected();
      }
      else
      {
        this->getSelectionManager().cutSelected();
      }
      return true;

    case CommandId::copy:
      if (auto focusedSelection =
              this->getEngine()
                  .getUIBehaviour()
                  .getCurrentlyFocusedSelectionManager())
      {
        focusedSelection->copySelected();
      }
      else
      {
        this->getSelectionManager().copySelected();
      }
      return true;

    case CommandId::paste:
      if (auto focusedSelection =
              this->getEngine()
                  .getUIBehaviour()
                  .getCurrentlyFocusedSelectionManager())
      {
        focusedSelection->pasteSelected();
      }
      else
      {
        this->getSelectionManager().pasteSelected();
      }
      return true;


    case CommandId::addTrack:
      this->getEditManager().add();
      return true;

    case CommandId::addPlugin:
      if (this->getEngine()
              .getUIBehaviour()
              .getCurrentlyFocusedSelectionManager()
              ->getFirstItemOfType<te::Track>())
      {
        PluginPickerComponent::Options pluginPickerOptions{};

        auto plugin = pickPlugin(*this, move(pluginPickerOptions));

        for (auto track :
             this->getEngine()
                 .getUIBehaviour()
                 .getCurrentlyFocusedSelectionManager()
                 ->getItemsOfType<EditTrack>())
        {
          auto& list = track->getAudio().pluginList;
          list.insertPlugin(
              plugin,
              list.size(),
              nullptr);
        }
      }
      return true;


    case CommandId::undo:
      this->getUndoManager().undo();
      return true;

    case CommandId::redo:
      this->getUndoManager().redo();
      return true;


    case CommandId::deselectAll:
      if (auto focusedSelection =
              this->getEngine()
                  .getUIBehaviour()
                  .getCurrentlyFocusedSelectionManager())
      {
        focusedSelection->deselectAll();
      }
      return true;


      // Transport

    case CommandId::play:
      for (auto track :
           this->getEngine()
               .getUIBehaviour()
               .getCurrentlyFocusedSelectionManager()
               ->getItemsOfType<EditTrack>())
        track->playback = TrackPlayback::playing;
      return true;

    case CommandId::pause:
      for (auto track :
           this->getEngine()
               .getUIBehaviour()
               .getCurrentlyFocusedSelectionManager()
               ->getItemsOfType<EditTrack>())
        track->playback = TrackPlayback::paused;
      return true;

    case CommandId::stop:
      for (auto track :
           this->getEngine()
               .getUIBehaviour()
               .getCurrentlyFocusedSelectionManager()
               ->getItemsOfType<EditTrack>())
        track->playback = TrackPlayback::paused;
      return true;

    case CommandId::record:
      for (auto track :
           this->getEngine()
               .getUIBehaviour()
               .getCurrentlyFocusedSelectionManager()
               ->getItemsOfType<EditTrack>())
        track->playback = TrackPlayback::recording;
      return true;


    case CommandId::toggleMonitoring:
      {
        auto& deviceManager = this->getEngine().getDeviceManager();
        for (int i = 0; i < deviceManager.getNumInputDevices(); ++i)
          if (auto input = deviceManager.getInputDevice(i))
            input->flipEndToEnd();
        return true;
      }


      // Track

    case CommandId::muteTrack:
      for (auto track :
           this->getEngine()
               .getUIBehaviour()
               .getCurrentlyFocusedSelectionManager()
               ->getItemsOfType<EditTrack>())
        util::toggle(track->muted);
      return true;

    case CommandId::soloTrack:
      if (auto track =
              this->getEngine()
                  .getUIBehaviour()
                  .getCurrentlyFocusedSelectionManager()
                  ->getFirstItemOfType<EditTrack>())
      {
        this->getEditManager().soloed = track->getId();
      }
      return true;

    case CommandId::armTrack:
      for (auto track :
           this->getEngine()
               .getUIBehaviour()
               .getCurrentlyFocusedSelectionManager()
               ->getItemsOfType<EditTrack>())
        util::toggle(track->armed);
      return true;


      // Parameter

    case CommandId::nudgeUp:
      for (auto parameter :
           this->getEngine()
               .getUIBehaviour()
               .getCurrentlyFocusedSelectionManager()
               ->getItemsOfType<te::AutomatableParameter>())
      {
        parameter->setNormalisedParameter(
            parameter->getCurrentValue() +
                parameter->getValueRange().getLength() / 20,
            juce::sendNotificationAsync);
      }
      return true;

    case CommandId::nudgeDown:
      for (auto parameter :
           this->getEngine()
               .getUIBehaviour()
               .getCurrentlyFocusedSelectionManager()
               ->getItemsOfType<te::AutomatableParameter>())
      {
        parameter->setNormalisedParameter(
            parameter->getCurrentValue() -
                parameter->getValueRange().getLength() / 20,
            juce::sendNotificationAsync);
      }
      return true;


    default:
      break;
  }


  bool             actionPerformed = false;
  juce::Array<int> deletedTargets{};

  for (int i = 0; i < this->commandTargets.size(); ++i)
  {
    const auto& target = this->commandTargets[i];

    if (target.wasObjectDeleted())
    {
      deletedTargets.add(i);
      continue;
    }

    if (auto targetPerformer = target->getTargetForCommand(command.commandID))
      if (targetPerformer->perform(command))
        actionPerformed = true;
  }

  for (auto deleted : deletedTargets)
    this->commandTargets.remove(deleted);

  return actionPerformed;
}

BLOOPER_NAMESPACE_END
