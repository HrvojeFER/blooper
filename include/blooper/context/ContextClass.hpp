#ifndef BLOOPER_CONTEXT_CLASS_HPP
#define BLOOPER_CONTEXT_CLASS_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class Context :
    public virtual AbstractContext
{
 public:
  BLOOPER_STATE_ID(Context);


  [[maybe_unused]] inline constexpr static auto fileKey =
      "context";


  [[maybe_unused]] inline constexpr static auto rootDirSpecialLocation =
      JuceFile::SpecialLocationType::userHomeDirectory;

  [[maybe_unused]] inline constexpr static auto rootDirName =
      ".blooper";


  [[maybe_unused]] inline constexpr static auto engineSettingsFileName =
      "engineSettings.xml";

  [[maybe_unused]] inline constexpr static auto settingsFileName =
      "settings.xml";

  [[maybe_unused]] inline constexpr static auto stateFileName =
      "state.xml";


  [[maybe_unused]] inline constexpr static auto logDirName =
      "log";

  [[maybe_unused]] inline constexpr static auto logFileNamePrefix =
      "blooper_";

  [[maybe_unused]] inline constexpr static auto logFileNameSuffix =
      ".log";


  [[maybe_unused]] inline constexpr static auto projectsDirName =
      "projects";


  struct Options
  {
    Options();

    std::function<void()> quit;

    std::function<void()> onLoad;
    std::function<void()> afterLoad;
    std::function<void()> onUnload;
    std::function<void()> afterUnload;

    std::function<void()> onEngineLoad;
    std::function<void()> afterEngineLoad;
    std::function<void()> onEngineUnload;
    std::function<void()> afterEngineUnload;

    std::function<void()> onProjectLoad;
    std::function<void()> afterProjectLoad;
    std::function<void()> onProjectUnload;
    std::function<void()> afterProjectUnload;
  } options;

  explicit Context(Options options = Options());

  ~Context() override;


  // Stateful

  [[maybe_unused, nodiscard]] inline const State&
  getState() const noexcept final;

  [[maybe_unused, nodiscard]] inline State&
  getState() noexcept final;


  // Context

  [[maybe_unused, nodiscard]] inline const JuceFile&
  getRootDir() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceFile&
  getRootDir() noexcept final;

  [[maybe_unused, nodiscard]] inline const JuceFile&
  getProjectsDir() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceFile&
  getProjectsDir() noexcept final;


  [[maybe_unused, nodiscard]] inline const JuceState&
  getEngineSettings() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceState&
  getEngineSettings() noexcept final;

  [[maybe_unused]] void
  saveEngineSettings() final;

  [[maybe_unused, nodiscard]] inline const JuceState&
  getSettings() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceState&
  getSettings() noexcept final;


  [[maybe_unused, nodiscard]] inline const JuceLogger&
  getLogger() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceLogger&
  getLogger() noexcept final;

  [[maybe_unused]] inline void
  log(const juce::String& message) final;


  [[maybe_unused, nodiscard]] inline const JuceUndoManager&
  getUndoManager() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceUndoManager&
  getUndoManager() noexcept final;

  [[maybe_unused, nodiscard]] inline const JuceUndoManager*
  getUndoManagerPtr() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceUndoManager*
  getUndoManagerPtr() noexcept final;


  [[maybe_unused, nodiscard]] inline const JuceCommandManager&
  getCommandManager() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceCommandManager&
  getCommandManager() noexcept final;

  [[maybe_unused]] inline void
  registerCommandTarget(JuceCommandTarget* target) final;

  [[maybe_unused]] inline void
  unregisterCommandTarget(JuceCommandTarget* target) final;


  [[maybe_unused, nodiscard]] inline const JuceLookAndFeel&
  getLookAndFeel() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceLookAndFeel&
  getLookAndFeel() noexcept final;


  // Engine

  [[maybe_unused, nodiscard]] inline const class AssetManager&
  getAssetManager() const noexcept final;

  [[maybe_unused, nodiscard]] inline class AssetManager&
  getAssetManager() noexcept final;


  [[maybe_unused, nodiscard]] inline const JuceEngine&
  getEngine() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceEngine&
  getEngine() noexcept final;


  [[maybe_unused, nodiscard]] inline const JuceSelectionManager&
  getSelectionManager() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceSelectionManager&
  getSelectionManager() noexcept final;


  // Project

  [[maybe_unused, nodiscard]] inline const JuceProject&
  getProject() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceProject&
  getProject() noexcept final;

  [[maybe_unused, nodiscard]] inline JuceProjectConstRef
  getProjectRef() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceProjectRef
  getProjectRef() noexcept final;

  [[maybe_unused, nodiscard]] inline const JuceState&
  getProjectSettings() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceState&
  getProjectSettings() noexcept final;

  [[maybe_unused, nodiscard]] inline const JuceState&
  getProjectState() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceState&
  getProjectState() noexcept final;


  [[maybe_unused, nodiscard]] inline const class EditManager&
  getEditManager() const noexcept final;

  [[maybe_unused, nodiscard]] inline class EditManager&
  getEditManager() noexcept final;


  [[maybe_unused, nodiscard]] inline const class Synchronizer&
  getSynchronizer() const noexcept final;

  [[maybe_unused, nodiscard]] inline class Synchronizer&
  getSynchronizer() noexcept final;


  // Special, Async

  [[maybe_unused]] void startAsync();

  [[maybe_unused]] void pickOrLoadProjectAsync();
  [[maybe_unused]] void pickProjectAsync();

  [[maybe_unused]] void saveEverythingAsync();

  [[maybe_unused]] void quitAsync();


  // Special, Sync

  [[maybe_unused]] void pickOrLoadProject();
  [[maybe_unused]] void pickProject();

  [[maybe_unused]] void saveEverything();

  [[maybe_unused]] void quit();


  // Safe, Async

  [[maybe_unused]] void loadAsync();
  [[maybe_unused]] void saveAsync();
  [[maybe_unused]] void unloadAsync();

  [[maybe_unused]] void loadEngineAsync(JuceString name);
  [[maybe_unused]] void saveEngineAsync();
  [[maybe_unused]] void unloadEngineAsync();

  [[maybe_unused]] void loadProjectAsync(JuceProjectRef ref);
  [[maybe_unused]] void saveProjectAsync();
  [[maybe_unused]] void unloadProjectAsync();


  // Safe, Sync

  [[maybe_unused]] void load();
  [[maybe_unused]] void unload();
  [[maybe_unused]] void save();

  [[maybe_unused]] void loadEngine(JuceString name);
  [[maybe_unused]] void unloadEngine();
  [[maybe_unused]] void saveEngine();

  [[maybe_unused]] void loadProject(JuceProjectRef ref);
  [[maybe_unused]] void unloadProject();
  [[maybe_unused]] void saveProject();


  // Unsafe, sync

  [[maybe_unused]] bool didLoad() const;
  [[maybe_unused]] bool didLoadEngine() const;
  [[maybe_unused]] bool didLoadProject() const;


 private:
  using Lock = juce::SpinLock;
  using ScopedLock = Lock::ScopedLockType;

  Lock lock;


  std::unique_ptr<JuceFile> rootDir;

  std::unique_ptr<JuceFile> projectsDir;

  std::unique_ptr<JuceXmlFile> engineSettingsFile;
  JuceState                    engineSettings;

  std::unique_ptr<JuceXmlFile> settingsFile;
  JuceState                    settings;

  std::unique_ptr<JuceXmlFile> stateFile;
  JuceState                    state;

  JuceCached<JuceString> openProjectFilePath;

  std::unique_ptr<JuceFile>         logDir;
  std::unique_ptr<JuceFile>         logFile;
  std::unique_ptr<juce::FileLogger> logger;

  std::unique_ptr<JuceUndoManager> undoManager;

  std::unique_ptr<JuceCommandManager>                 commandManager;
  juce::Array<juce::WeakReference<JuceCommandTarget>> commandTargets;

  std::unique_ptr<JuceLookAndFeel> lookAndFeel;

  bool loaded;


  std::unique_ptr<class AssetManager> assetManager;

  std::unique_ptr<juce::TooltipWindow> tooltipWindow;

  std::unique_ptr<JuceEngine> engine;
  JuceCached<bool>            monitored;

  std::unique_ptr<JuceSelectionManager> selectionManager;

  bool loadedEngine;


  JuceProjectRef project;

  [[maybe_unused]] JuceProjectItemRef projectSettingsItem;
  std::unique_ptr<JuceXmlFile>        projectSettingsFile;
  JuceState                           projectSettings;

  [[maybe_unused]] JuceProjectItemRef projectStateItem;
  std::unique_ptr<JuceXmlFile>        projectStateFile;
  JuceState                           projectState;

  std::unique_ptr<class Synchronizer> synchronizer;

  std::unique_ptr<class EditManager> editManager;

  bool loadedProject;


  // Special, Async

  void reloadProjectAsync(JuceProjectRef ref);
  void reloadProjectAsync();


  // Special, Sync

  void reloadProject(JuceProjectRef ref);
  void reloadProject();


  // Unsafe, Sync

  void loadUnsafe();
  void unloadUnsafe();
  void saveUnsafe();

  void loadEngineUnsafe(JuceString name);
  void unloadEngineUnsafe();
  void saveEngineUnsafe();

  void loadProjectUnsafe(JuceProjectRef ref);
  void unloadProjectUnsafe();
  void saveProjectUnsafe();


  // CommandTarget

 private:
  ApplicationCommandTarget* getNextCommandTarget() override;

  void getAllCommands(juce::Array<JuceCommandId>& commands) override;

  void getCommandInfo(JuceCommandId id, JuceCommandInfo& info) override;

  bool perform(const JuceCommand& command) override;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Context)
  JUCE_DECLARE_WEAK_REFERENCEABLE(Context)
};


// Stateful

const State& Context::getState() const noexcept
{
  return state;
}

State& Context::getState() noexcept
{
  return state;
}


// Context

const JuceFile& Context::getRootDir() const noexcept
{
  return *this->rootDir;
}

JuceFile& Context::getRootDir() noexcept
{
  return *this->rootDir;
}

const JuceFile& Context::getProjectsDir() const noexcept
{
  return *this->projectsDir;
}

JuceFile& Context::getProjectsDir() noexcept
{
  return *this->projectsDir;
}


const JuceState& Context::getEngineSettings() const noexcept
{
  return this->engineSettings;
}

JuceState& Context::getEngineSettings() noexcept
{
  return this->engineSettings;
}

const JuceState& Context::getSettings() const noexcept
{
  return this->settings;
}

JuceState& Context::getSettings() noexcept
{
  return this->settings;
}


const JuceLogger& Context::getLogger() const noexcept
{
  return *this->logger;
}

JuceLogger& Context::getLogger() noexcept
{
  return *this->logger;
}

void Context::log(const JuceString& message)
{
  BLOOPER_ASSERT(this->loadedEngine);

  this->logger->logMessage(message);
}


const JuceUndoManager& Context::getUndoManager() const noexcept
{
  return *this->undoManager;
}

JuceUndoManager& Context::getUndoManager() noexcept
{
  return *this->undoManager;
}

const JuceUndoManager* Context::getUndoManagerPtr() const noexcept
{
  return this->undoManager.get();
}

JuceUndoManager* Context::getUndoManagerPtr() noexcept
{
  return this->undoManager.get();
}


const JuceCommandManager& Context::getCommandManager() const noexcept
{
  return *this->commandManager;
}

JuceCommandManager& Context::getCommandManager() noexcept
{
  return *this->commandManager;
}

void Context::registerCommandTarget(JuceCommandTarget* target)
{
  if (!target) return;

  this->commandTargets.add(target);
  this->getCommandManager().registerAllCommandsForTarget(this);
}

void Context::unregisterCommandTarget(JuceCommandTarget* target)
{
  if (!target) return;

  this->commandTargets.removeFirstMatchingValue(target);
}


const JuceLookAndFeel& Context::getLookAndFeel() const noexcept
{
  return *this->lookAndFeel;
}

JuceLookAndFeel& Context::getLookAndFeel() noexcept
{
  return *this->lookAndFeel;
}


// Engine

const class AssetManager& Context::getAssetManager() const noexcept
{
  return *this->assetManager;
}

class AssetManager& Context::getAssetManager() noexcept
{
  return *this->assetManager;
}


const JuceEngine& Context::getEngine() const noexcept
{
  return *this->engine;
}

JuceEngine& Context::getEngine() noexcept
{
  return *this->engine;
}


const JuceSelectionManager& Context::getSelectionManager() const noexcept
{
  return *this->selectionManager;
}

JuceSelectionManager& Context::getSelectionManager() noexcept
{
  return *this->selectionManager;
}


// Project

const JuceProject& Context::getProject() const noexcept
{
  return *this->project;
}

JuceProject& Context::getProject() noexcept
{
  return *this->project;
}

JuceProjectConstRef Context::getProjectRef() const noexcept
{
  return this->project;
}

JuceProjectRef Context::getProjectRef() noexcept
{
  return this->project;
}


const JuceState& Context::getProjectSettings() const noexcept
{
  return this->projectSettings;
}

JuceState& Context::getProjectSettings() noexcept
{
  return this->projectSettings;
}


const JuceState& Context::getProjectState() const noexcept
{
  return this->projectState;
}

JuceState& Context::getProjectState() noexcept
{
  return this->projectState;
}


const class EditManager& Context::getEditManager() const noexcept
{
  return *this->editManager;
}

class EditManager& Context::getEditManager() noexcept
{
  return *this->editManager;
}


const class Synchronizer& Context::getSynchronizer() const noexcept
{
  return *this->synchronizer;
}

class Synchronizer& Context::getSynchronizer() noexcept
{
  return *this->synchronizer;
}


BLOOPER_STATIC_ASSERT(
    meta::not_(meta::traits::is_abstract(meta::type_c<Context>)),
    "Context must be concrete.");

BLOOPER_STATIC_ASSERT(
    isContext(meta::type_c<Context>),
    "Context must satisfy Context.");

BLOOPER_NAMESPACE_END

#endif // BLOOPER_CONTEXT_CLASS_HPP
