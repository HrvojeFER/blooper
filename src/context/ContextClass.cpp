#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

Context::Context(
    JuceString name,
    Options    options)
    : options(move(options))
{
  this->load(move(name));
  this->setup();
}

Context::~Context()
{
  this->save();
  this->unload();
}


// TODO: Utils - put in util or ext?

namespace
{
inline std::unique_ptr<JuceFile> ensureExistingDirectory(const JuceFile& file)
{
  if (!file.exists()) file.createDirectory();

  return std::make_unique<JuceFile>(file);
}

inline std::unique_ptr<JuceFile> ensureExistingFile(const JuceFile& file)
{
  if (!file.existsAsFile()) file.create();

  return std::make_unique<JuceFile>(file);
}

inline std::unique_ptr<JuceXmlFile> ensureValidStateFile(
    const JuceFile& file)
{
  JuceXmlFile::Options options;
  options.millisecondsBeforeSaving = 2000;
  options.storageFormat = JuceXmlFile::storeAsXML;
  options.commonToAllUsers = false;

  // If not valid, just delete it and it will create a new one when needed.
  if (!JuceXmlFile(file, options).isValidFile())
    file.deleteFile();

  return std::make_unique<JuceXmlFile>(file, options);
}

inline JuceState ensureValidState(
    JuceXmlFile&               file,
    const JuceString&          key,
    const JuceStateIdentifier& id)
{
  JuceState state{};

  auto xml = file.getXmlValue(key);

  if (!xml)
  {
    state = State(id);
    xml = state.createXml();

    file.setValue(key, xml.get());
  }
  else
  {
    state = JuceState::fromXml(*xml);
  }


  if (!state.isValid() || !state.hasType(id))
  {
    state = State(id);
    file.setValue(key, state.createXml().get());
  }

  return state;
}
} // namespace


// Core

[[maybe_unused]] bool Context::didLoad()
{
  return this->engine != nullptr;
}

[[maybe_unused]] void Context::load(JuceString name)
{
  this->rootDir =
      ensureExistingDirectory(
          JuceFile::getSpecialLocation(
              Context::rootDirSpecialLocation)
              .getChildFile(Context::rootDirName));

  this->projectsDir =
      ensureExistingDirectory(
          this->getRootDir().getChildFile(
              Context::projectsDirName));

  this->propertiesFile =
      ensureValidStateFile(
          this->getRootDir().getChildFile(
              Context::propertiesFileName));

  this->settingsFile =
      ensureValidStateFile(
          this->getRootDir().getChildFile(
              Context::settingsFileName));
  this->settings =
      ensureValidState(
          *this->settingsFile,
          Context::fileKey,
          Context::stateId);

  this->stateFile =
      ensureValidStateFile(
          this->getRootDir().getChildFile(
              Context::stateFileName));
  this->state =
      ensureValidState(
          *this->stateFile,
          Context::fileKey,
          Context::stateId);

  this->openProjectFilePath.referTo(
      this->getState(),
      Context::openProjectId,
      std::addressof(this->getUndoManager()),
      "");


  this->logDir =
      ensureExistingDirectory(
          this->getRootDir().getChildFile(Context::logDirName));

  this->logFile =
      ensureExistingFile(
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

  this->lookAndFeel = std::make_unique<LookAndFeel>(*this);
  JuceLookAndFeel::setDefaultLookAndFeel(
      this->lookAndFeel.get());

  this->engine =
      std::make_unique<JuceEngine>(
          std::make_unique<PropertyStorage>(move(name), *this),
          std::make_unique<UIBehaviour>(*this),
          std::make_unique<EngineBehaviour>(*this));

  this->getEngine().getProjectManager().loadList();

  this->selectionManager =
      std::make_unique<JuceSelectionManager>(
          this->getEngine());


  if (this->openProjectFilePath->isEmpty())
  {
    ProjectsMenuWindow::Options projectsMenuOptions{};

    projectsMenuOptions.onOpen =
        ([this](auto projectRef) {
          this->loadProject(move(projectRef));
        });

    projectsMenuOptions.onCancel =
        ([this] {
          this->options.onClose();
        });

    showProjectsMenu(*this, move(projectsMenuOptions));
  }
  else
  {
    auto& projectManager = this->getEngine().getProjectManager();

    this->loadProject(
        projectManager.findProjectWithFile(
            projectManager.folders,
            JuceFile{this->openProjectFilePath}));
  }

  JuceLogger::writeToLog("Loaded context.");
}

[[maybe_unused]] void Context::setup()
{
  if (!this->didLoad()) return;

  // TODO: commands

  this->setupProject();

  JuceLogger::writeToLog("Setup context.");
}

[[maybe_unused]] void Context::save()
{
  if (!this->didLoad()) return;


  this->saveProject();


  this->engine->getProjectManager().saveList();

  this->engine->getTemporaryFileManager()
      .getTempDirectory()
      .deleteRecursively();


  this->settingsFile->setValue(
      Context::fileKey,
      this->getSettings().createXml().get());
  this->settingsFile->save();

  this->stateFile->setValue(
      Context::fileKey,
      this->getState().createXml().get());
  this->stateFile->save();


  JuceLogger::writeToLog("Saved context.");
}

[[maybe_unused]] void Context::unload()
{
  if (!this->didLoad()) return;

  JuceLogger::writeToLog("Unloading context.");


  this->unloadProject();


  this->selectionManager.reset();
  this->engine.reset();

  this->lookAndFeel.reset();

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

  this->propertiesFile.reset();

  this->projectsDir.reset();

  this->rootDir.reset();
}


// Project

[[maybe_unused]] bool Context::didLoadProject()
{
  return this->project != nullptr;
}

[[maybe_unused]] void Context::loadProject(JuceProjectRef ref)
{
  if (!ref) return;


  this->project = move(ref);

  this->getState().setProperty(
      Context::openProjectId,
      this->getProject().getProjectFile().getFullPathName(),
      std::addressof(this->getUndoManager()));


  this->projectSettingsFile =
      ensureValidStateFile(
          this->getProject()
              .getProjectFile()
              .getParentDirectory()
              .getChildFile(
                  Context::settingsFileName));

  const auto& normalSettingsFile = this->projectSettingsFile->getFile();

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
      ensureValidState(
          *this->projectSettingsFile,
          Context::fileKey,
          Context::stateId);


  this->projectStateFile =
      ensureValidStateFile(
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
      ensureValidState(
          *this->projectStateFile,
          Context::fileKey,
          Context::stateId);


  this->edit = ext::ensureEdit(
      this->getProject(),
      this->getEngine());

  this->transport = &this->getEdit().getTransport();


  juce::MessageManager::callAsync(
      [context = juce::WeakReference<Context>(this)] {
        if (!context.wasObjectDeleted())
          context->options.onProjectLoad();
      });


  JuceLogger::writeToLog("Loaded project.");
}

[[maybe_unused]] void Context::setupProject()
{
  JuceLogger::writeToLog("Setup project.");
}


[[maybe_unused]] void Context::saveProject()
{
  if (!this->didLoadProject()) return;


  te::EditFileOperations(*this->edit)
      .save(true,
            true,
            true);


  this->projectSettingsFile->setValue(
      Context::fileKey,
      this->getProjectSettings().createXml().get());
  this->projectSettingsFile->save();

  this->projectStateFile->setValue(
      Context::fileKey,
      this->getProjectState().createXml().get());
  this->projectStateFile->save();


  this->project->save();


  JuceLogger::writeToLog("Saved project.");
}

[[maybe_unused]] void Context::unloadProject()
{
  if (!this->didLoadProject()) return;

  JuceLogger::writeToLog("Unloading project.");


  this->transport = nullptr;
  this->edit.reset();


  this->projectState = {};
  this->projectStateFile.reset();

  this->projectSettings = {};
  this->projectSettingsFile.reset();


  this->getState().setProperty(
      Context::openProjectId,
      "",
      std::addressof(this->getUndoManager()));

  this->project.reset();


  juce::MessageManager::callAsync(
      [context = juce::WeakReference<Context>(this)] {
        if (!context.wasObjectDeleted())
          context->options.onProjectUnload();
      });
}

BLOOPER_NAMESPACE_END
