#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

juce::ValueTree ensureChildFolder(
    juce::ValueTree&    root,
    const juce::String& name)
{
  for (int i = 0; i < root.getNumChildren(); ++i)
  {
    auto&& child = root.getChild(i);
    if (child.hasType(te::IDs::FOLDER) &&
        child[te::IDs::name].toString() == name)
      return child;
  }

  juce::ValueTree folder(te::IDs::FOLDER);
  folder.setProperty(te::IDs::name, name, nullptr);
  root.addChild(folder, 0, nullptr);
  ext::ensureAllItemsHaveIDs(root);

  return folder;
}

juce::ValueTree ensureProjectPath(
    te::Engine&         engine,
    const juce::String& path)
{
  auto& manager = engine.getProjectManager();
  auto  root = manager.folders;

  auto splitPath = ext::splitPath(path);

  for (const auto& i : splitPath)
    root = ensureChildFolder(root, i);

  return root;
}


[[maybe_unused]] ProjectsMenuComponent::ProjectsMenuComponent(
    AbstractCoreContext& context,
    State                state,
    Options              options)
    : CoreComponentBase(
          context,
          move(state)),
      options(move(options)),


      isAddingProject(
          this->getState(),
          isAddingProjectId,
          std::addressof(this->getContext().getUndoManager()),
          false),

      projectPath(
          this->getState(),
          projectPathId,
          std::addressof(this->getContext().getUndoManager()),
          ""),
      projectFile(
          this->getState(),
          projectFileId,
          std::addressof(this->getContext().getUndoManager()),
          ""),


      projects(
          ([this] {
            auto& manager =
                this->getContext()
                    .getEngine()
                    .getProjectManager();

            manager.loadList();
            return manager.getAllProjects(manager.folders);
          }) ()),

      list("Projects", this),


      reloadProjectsButton("Reload"),
      addProjectButton("Add..."),
      deleteProjectButton("Delete"),

      cancelButton("Cancel"),
      openProjectButton("Open..."),

      addProjectPanel("Add project...")
{
  list.setMultipleSelectionEnabled(false);
  list.setRowSelectedOnMouseDown(true);
  list.setRowHeight(30);


  addProjectPanel.addProperties(
      {new juce::TextPropertyComponent(
           projectPath.getPropertyAsValue(),
           "Folder",
           200,
           false,
           true),

       new juce::TextPropertyComponent(
           projectFile.getPropertyAsValue(),
           "File",
           200,
           false,
           true)},

      2);


  reloadProjectsButton.onClick = [this] {
    this->reloadProjects();
    this->repaint();
  };

  addProjectButton.onClick = [this] {
    if (this->isAddingProject)
    {
      auto& manager =
          this->getContext()
              .getEngine()
              .getProjectManager();

      manager.createNewProject(
          this->projectFile.get(),
          ensureProjectPath(
              this->getContext().getEngine(),
              this->projectPath.get()));

      reloadProjects();
    }

    this->toggleAddingProject();
  };

  deleteProjectButton.onClick = [this] {
    auto row = list.getSelectedRow();
    if (row == -1) return;

    //    auto& manager =
    //        this->getContext()
    //            .getEngine()
    //            .getProjectManager();

    //    manager.deleteProjectFolder(projects[row]);

    this->reloadProjects();
    this->repaint();
  };

  cancelButton.onClick = [this] {
    if (!this->isAddingProject)
    {
      this->options.onCancel();
    }
    else
    {
      this->toggleAddingProject();
    }
  };

  openProjectButton.onClick = [this] {
    auto row = list.getSelectedRow();
    if (row == -1) return;

    this->options.onOpen(projects[row]);
  };


  ext::addAndMakeVisible(
      *this,
      list,

      reloadProjectsButton,
      addProjectButton,
      deleteProjectButton,

      cancelButton,
      openProjectButton,

      addProjectPanel);

  addProjectPanel.setVisible(false);
}


void ProjectsMenuComponent::resized()
{
  auto availableArea =
      util::buildBar(
          this->getLookAndFeel(),
          this->getLocalBounds())
          .addComponentsRight(
              openProjectButton,
              cancelButton)
          .addComponentsLeft(
              reloadProjectsButton,
              addProjectButton,
              deleteProjectButton)
          .finish();

  if (isAddingProject)
  {
    addProjectPanel.setBounds(
        availableArea.removeFromBottom(
            availableArea.getHeight() / 10));
  }

  list.setBounds(availableArea);
}


[[maybe_unused]] void ProjectsMenuComponent::toggleAddingProject()
{
  if (this->isAddingProject)
  {
    this->isAddingProject = false;
    this->addProjectButton.setButtonText("Add...");
    this->addProjectPanel.setVisible(false);
  }
  else
  {
    this->isAddingProject = true;
    this->addProjectButton.setButtonText("Add");
    this->addProjectPanel.setVisible(true);
  }

#ifdef __JETBRAINS_IDE__
  #pragma clang diagnostic push
  #pragma ide diagnostic   ignored "VirtualCallInCtorOrDtor"
#endif

  this->resized();
  this->repaint();

#ifdef __JETBRAINS_IDE__
  #pragma clang diagnostic pop
#endif
}

[[maybe_unused]] void ProjectsMenuComponent::reloadProjects()
{
  auto& manager = this->getContext().getEngine().getProjectManager();

  manager.saveList();
  manager.loadList();
  projects = manager.getAllProjects(manager.folders);
}


[[maybe_unused]] int ProjectsMenuComponent::getNumRows()
{
  return projects.size();
}

[[maybe_unused]] void ProjectsMenuComponent::paintListBoxItem(
    int             rowNumber,
    juce::Graphics& g,
    int             width,
    int             height,
    bool            rowIsSelected)
{
  if (rowNumber < 0 || rowNumber >= projects.size()) return;

  auto project = projects[rowNumber];


  auto availableArea = juce::Rectangle<int>(
      0,
      0,
      width,
      height);


  g.fillAll(this->getLookAndFeel().findColour(
      juce::ResizableWindow::backgroundColourId));

  if (rowIsSelected)
    g.setColour(juce::Colours::lightgrey);
  else
    g.setColour(juce::Colours::white);

  g.drawText(project->getName(),
             availableArea,
             juce::Justification::left);
}


[[maybe_unused]] void ProjectsMenuComponent::listBoxItemClicked(
    int                     row,
    const juce::MouseEvent& event)
{
  ListBoxModel::listBoxItemClicked(row, event);
}

[[maybe_unused]] void ProjectsMenuComponent::listBoxItemDoubleClicked(
    int row,
    const juce::MouseEvent&)
{
  if (row == -1) return;
  this->options.onOpen(projects[row]);
}

[[maybe_unused]] void ProjectsMenuComponent::deleteKeyPressed(
    int)
{
  auto& manager = this->getContext().getEngine().getProjectManager();

  //  manager.deleteProjectFolder(projects[lastRowSelected]);
  projects = manager.getAllProjects(manager.folders);

  this->repaint();
}

BLOOPER_NAMESPACE_END
