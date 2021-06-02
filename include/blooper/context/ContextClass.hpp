#ifndef BLOOPER_CONTEXT_CLASS_HPP
#define BLOOPER_CONTEXT_CLASS_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class Context : public AbstractContext
{
 public:
  BLOOPER_STATE_ID(Context);

  BLOOPER_ID(openProjectId);


  [[maybe_unused]] inline constexpr static auto fileKey =
      "context";


  [[maybe_unused]] inline constexpr static auto rootDirSpecialLocation =
      JuceFile::SpecialLocationType::userHomeDirectory;

  [[maybe_unused]] inline constexpr static auto rootDirName =
      ".blooper";


  [[maybe_unused]] inline constexpr static auto propertiesFileName =
      "properties.xml";

  [[maybe_unused]] inline constexpr static auto settingsFileName =
      "settings.xml";

  [[maybe_unused]] inline constexpr static auto stateFileName =
      "state.xml";


  [[maybe_unused]] inline constexpr static auto logDirName =
      "log";

  [[maybe_unused]] inline constexpr static auto logFileNameSuffix =
      ".log";


  [[maybe_unused]] inline constexpr static auto projectsDirName =
      "projects";


  struct Options
  {
    std::function<void()> onProjectLoad;
    std::function<void()> onProjectUnload;
    std::function<void()> onClose;
  } options;

  explicit Context(
      JuceString name,
      Options    options = {});

  ~Context() override;


  // Core

  [[maybe_unused, nodiscard]] inline const JuceFile&
  getRootDir() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceFile&
  getRootDir() noexcept final;


  [[maybe_unused, nodiscard]] inline const JuceFile&
  getProjectsDir() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceFile&
  getProjectsDir() noexcept final;


  [[maybe_unused, nodiscard]] inline const JuceXmlFile&
  getProperties() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceXmlFile&
  getProperties() noexcept final;


  [[maybe_unused, nodiscard]] inline const JuceState&
  getSettings() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceState&
  getSettings() noexcept final;


  [[maybe_unused, nodiscard]] inline const State&
  getState() const noexcept final;

  [[maybe_unused, nodiscard]] inline State&
  getState() noexcept final;


  [[maybe_unused, nodiscard]] inline const JuceLookAndFeel&
  getLookAndFeel() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceLookAndFeel&
  getLookAndFeel() noexcept final;


  [[maybe_unused, nodiscard]] inline const JuceUndoManager&
  getUndoManager() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceUndoManager&
  getUndoManager() noexcept final;


  [[maybe_unused, nodiscard]] inline const JuceCommandManager&
  getCommandManager() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceCommandManager&
  getCommandManager() noexcept final;


  [[maybe_unused, nodiscard]] inline const JuceLogger&
  getLogger() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceLogger&
  getLogger() noexcept final;


  [[maybe_unused, nodiscard]] inline const JuceEngine&
  getEngine() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceEngine&
  getEngine() noexcept final;


  // Non-core

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


  [[maybe_unused, nodiscard]] inline const JuceEdit&
  getEdit() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceEdit&
  getEdit() noexcept final;


  [[maybe_unused, nodiscard]] inline const JuceTransport&
  getTransport() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceTransport&
  getTransport() noexcept final;


 private:
  [[maybe_unused]] std::unique_ptr<JuceFile> rootDir;

  [[maybe_unused]] std::unique_ptr<JuceFile> projectsDir;

  [[maybe_unused]] std::unique_ptr<JuceXmlFile> propertiesFile;

  [[maybe_unused]] std::unique_ptr<JuceXmlFile> settingsFile;
  [[maybe_unused]] JuceState                    settings;

  [[maybe_unused]] std::unique_ptr<JuceXmlFile> stateFile;
  [[maybe_unused]] JuceState                    state;

  [[maybe_unused]] JuceCached<JuceString> openProjectFilePath;


  [[maybe_unused]] std::unique_ptr<JuceFile>   logDir;
  [[maybe_unused]] std::unique_ptr<JuceFile>   logFile;
  [[maybe_unused]] std::unique_ptr<JuceLogger> logger;

  [[maybe_unused]] std::unique_ptr<JuceUndoManager>    undoManager;
  [[maybe_unused]] std::unique_ptr<JuceCommandManager> commandManager;

  [[maybe_unused]] std::unique_ptr<JuceLookAndFeel> lookAndFeel;

  [[maybe_unused]] std::unique_ptr<JuceEngine> engine;

  [[maybe_unused]] std::unique_ptr<JuceSelectionManager> selectionManager;


  [[maybe_unused]] JuceProjectRef project;

  [[maybe_unused]] JuceProjectItemRef           projectSettingsItem;
  [[maybe_unused]] std::unique_ptr<JuceXmlFile> projectSettingsFile;
  [[maybe_unused]] JuceState                    projectSettings;

  [[maybe_unused]] JuceProjectItemRef           projectStateItem;
  [[maybe_unused]] std::unique_ptr<JuceXmlFile> projectStateFile;
  [[maybe_unused]] JuceState                    projectState;

  [[maybe_unused]] std::unique_ptr<JuceEdit> edit;
  [[maybe_unused]] JuceTransport*            transport{nullptr};


  [[maybe_unused]] bool didLoad();
  [[maybe_unused]] void load(JuceString name);
  [[maybe_unused]] void save();
  [[maybe_unused]] void unload();

  [[maybe_unused]] bool didLoadProject();
  [[maybe_unused]] void loadProject(JuceProjectRef ref);
  [[maybe_unused]] void saveProject();
  [[maybe_unused]] void unloadProject();


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Context)
  JUCE_DECLARE_WEAK_REFERENCEABLE(Context)
};


// Core

const JuceFile& Context::getRootDir() const noexcept
{
  return *rootDir;
}

JuceFile& Context::getRootDir() noexcept
{
  return *rootDir;
}


const JuceFile& Context::getProjectsDir() const noexcept
{
  return *projectsDir;
}

JuceFile& Context::getProjectsDir() noexcept
{
  return *projectsDir;
}


const JuceXmlFile& Context::getProperties() const noexcept
{
  return *propertiesFile;
}

JuceXmlFile& Context::getProperties() noexcept
{
  return *propertiesFile;
}


const State& Context::getSettings() const noexcept
{
  return settings;
}

State& Context::getSettings() noexcept
{
  return settings;
}


const State& Context::getState() const noexcept
{
  return state;
}

State& Context::getState() noexcept
{
  return state;
}


const JuceLookAndFeel& Context::getLookAndFeel() const noexcept
{
  return *lookAndFeel;
}

JuceLookAndFeel& Context::getLookAndFeel() noexcept
{
  return *lookAndFeel;
}


const JuceUndoManager& Context::getUndoManager() const noexcept
{
  return *undoManager;
}

JuceUndoManager& Context::getUndoManager() noexcept
{
  return *undoManager;
}


const JuceCommandManager& Context::getCommandManager() const noexcept
{
  return *commandManager;
}

JuceCommandManager& Context::getCommandManager() noexcept
{
  return *commandManager;
}


const JuceLogger& Context::getLogger() const noexcept
{
  return *logger;
}

JuceLogger& Context::getLogger() noexcept
{
  return *logger;
}


const JuceEngine& Context::getEngine() const noexcept
{
  return *engine;
}

JuceEngine& Context::getEngine() noexcept
{
  return *engine;
}


// Non-core

const JuceProject& Context::getProject() const noexcept
{
  return *project;
}

JuceProject& Context::getProject() noexcept
{
  return *project;
}

JuceProjectConstRef Context::getProjectRef() const noexcept
{
  return project;
}

JuceProjectRef Context::getProjectRef() noexcept
{
  return project;
}


const JuceState& Context::getProjectSettings() const noexcept
{
  return projectSettings;
}

JuceState& Context::getProjectSettings() noexcept
{
  return projectSettings;
}


const JuceState& Context::getProjectState() const noexcept
{
  return projectState;
}

JuceState& Context::getProjectState() noexcept
{
  return projectState;
}


const JuceEdit& Context::getEdit() const noexcept
{
  return *edit;
}

JuceEdit& Context::getEdit() noexcept
{
  return *edit;
}


const JuceTransport& Context::getTransport() const noexcept
{
  return *transport;
}

JuceTransport& Context::getTransport() noexcept
{
  return *transport;
}


BLOOPER_STATIC_ASSERT(
    !std::is_abstract_v<Context>,
    "Context should be concrete.");

BLOOPER_NAMESPACE_END

#endif // BLOOPER_CONTEXT_CLASS_HPP
