#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

namespace
{
inline JuceXmlFile ensureValidStateFile(JuceEngine& engine)
{
  JuceXmlFile::Options options;
  options.millisecondsBeforeSaving = 2000;
  options.storageFormat = JuceXmlFile::storeAsXML;
  options.commonToAllUsers = false;

  auto stateFile =
      engine.getPropertyStorage()
          .getAppPrefsFolder()
          .getChildFile(Context::stateFileName);

  // If not valid, just delete it and it will create a new one when needed.
  if (!JuceXmlFile(stateFile, options).isValidFile())
    stateFile.deleteFile();

  return JuceXmlFile(stateFile, options);
}

inline State ensureValidState(JuceXmlFile& file)
{
  State state{};

  auto xml = file.getXmlValue(Context::stateFileKey);

  if (!xml)
  {
    state = State(Context::stateId);
    xml = state.createXml();

    file.setValue(Context::stateFileKey, xml.get());
  }
  else
  {
    state = State::fromXml(*xml);
  }


  if (!state.isValid() || !state.hasType(Context::stateId))
  {
    state = State(Context::stateId);
    file.setValue(Context::stateFileKey, state.createXml().get());
  }

  return state;
}
} // namespace

Context::Context(
    JuceString name,
    Options    options)
    : AbstractContext(),
      options(move(options)),

      engine(
          std::make_unique<PropertyStorage>(move(name), *this),
          std::make_unique<UIBehaviour>(*this),
          std::make_unique<EngineBehaviour>(*this)),

      stateFile(ensureValidStateFile(engine)),
      state(ensureValidState(stateFile)),

      lookAndFeel(std::make_unique<LookAndFeel>(*this)),

      undoManager(),

      project(),

      edit(),
      transport()
{
  engine.getProjectManager().loadList();


  auto openProject =
      this->getState()
          .getProperty(Context::openProjectId)
          .toString();

  if (openProject.isEmpty())
  {
    ProjectsMenuWindow::Options projectsMenuOptions{};

    projectsMenuOptions.onOpen =
        ([this](auto projectRef) {
          this->setProject(move(projectRef));

          this->options.onInitSuccess();
        });

    projectsMenuOptions.onCancel =
        ([this] {
          this->options.onInitFailure();
        });

    showProjectsMenu(*this, move(projectsMenuOptions));
  }
  else
  {
    auto& projectManager = this->getEngine().getProjectManager();

    this->setProject(
        projectManager.findProjectWithFile(
            projectManager.folders,
            openProject));

    juce::MessageManager::callAsync(
        [this] {
          this->options.onInitSuccess();
        });
  }
}

Context::~Context()
{
  if (edit != nullptr)
  {
    te::EditFileOperations(*edit)
        .save(true,
              true,
              true);
  }

  if (project != nullptr)
  {
    project->save();
  }

  stateFile.setValue(stateFileKey, state.createXml().get());
  stateFile.save();

  engine.getTemporaryFileManager()
      .getTempDirectory()
      .deleteRecursively();

  engine.getPropertyStorage().flushSettingsToDisk();
  engine.getProjectManager().saveList();
}


[[maybe_unused]] void Context::setProject(JuceProjectRef ref)
{
  if (ref)
  {
    this->project = move(ref);

    this->getState().setProperty(
        Context::openProjectId,
        this->getProject().getProjectFile().getFullPathName(),
        std::addressof(this->getUndoManager()));

    this->edit = ext::ensureEdit(
        this->getProject(),
        this->getEngine());

    this->transport = &this->getEdit().getTransport();
  }
  else
  {
    this->transport = nullptr;
    this->edit.reset();

    this->getState().setProperty(
        Context::openProjectId,
        "",
        std::addressof(this->getUndoManager()));

    this->project.reset();
  }
}

BLOOPER_NAMESPACE_END
