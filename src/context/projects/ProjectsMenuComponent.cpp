#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

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


  reloadProjectsButton.onClick = [this] {
    this->getContext().getEngine().getProjectManager().loadList();

    this->reloadProjects();
  };

  addProjectButton.onClick = [this] {
    if (this->isAddingProject)
    {
      util::addProject(
          this->getContext(),
          ext::splitPath(projectPath),
          JuceFile{projectFile});

      this->reloadProjects();
    }

    this->toggleAddingProject();
  };

  deleteProjectButton.onClick = [this] {
    auto row = list.getSelectedRow();
    if (!this->isValidRow(row)) return;

    util::deleteProject(
        this->getContext(),
        projects[row]);

    this->reloadProjects();
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
    if (!this->isValidRow(row)) return;

    this->options.onOpen(projects[row]);

    this->reloadProjects();
  };


  addProjectPanel.addProperties(
      {new juce::TextPropertyComponent(
           projectPath.getPropertyAsValue(),
           "Folder",
           200,
           false,
           true),

       new util::FilePathPropertyComponent(
           projectFile.getPropertyAsValue(),
           "File",
           false,
           true,
           "*.tracktion")},

      2);


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

  this->resized();
}

[[maybe_unused]] void ProjectsMenuComponent::reloadProjects()
{
  auto& manager = this->getContext().getEngine().getProjectManager();

  projects = manager.getAllProjects(manager.folders);

  this->list.updateContent();
}

[[maybe_unused]] bool ProjectsMenuComponent::isValidRow(int row) const noexcept
{
  return row >= 0 && row < this->projects.size();
}


[[maybe_unused]] int ProjectsMenuComponent::getNumRows()
{
  return projects.size();
}

[[maybe_unused]] void ProjectsMenuComponent::paintListBoxItem(
    int             row,
    juce::Graphics& g,
    int             width,
    int             height,
    bool            rowIsSelected)
{
  if (!this->isValidRow(row)) return;

  auto project = projects[row];


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
  if (!this->isValidRow(row)) return;

  ListBoxModel::listBoxItemClicked(row, event);
}

[[maybe_unused]] void ProjectsMenuComponent::listBoxItemDoubleClicked(
    int row,
    const juce::MouseEvent&)
{
  if (!this->isValidRow(row)) return;

  this->options.onOpen(projects[row]);

  this->reloadProjects();
}

[[maybe_unused]] void ProjectsMenuComponent::deleteKeyPressed(
    int row)
{
  if (!this->isValidRow(row)) return;

  util::deleteProject(
      this->getContext(),
      projects[row]);

  this->reloadProjects();
}

BLOOPER_NAMESPACE_END
