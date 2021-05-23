#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

Context::Context(const Options& options)
    : AbstractContext(),
      engine(
          std::make_unique<PropertyStorage>(*this),
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


  auto projectsMenuWindow = new ProjectsMenuWindow(*this);

  projectsMenuWindow->onOpen =
      ([this, options](auto projectRef) {
        this->project = std::move(projectRef);

        this->edit = ext::ensureEdit(
            this->getProject(),
            this->getEngine());

        this->transport = &getEdit().getTransport();

        options.onInitSuccess();
      });

  projectsMenuWindow->onCancel =
      ([options] {
        options.onInitFailure();
      });

  projectsMenuWindow->enterModalState(
      true,
      nullptr,
      true);
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

BLOOPER_NAMESPACE_END
