#include <blooper/context/projects/ProjectsMenuComponent.hpp>

#include <blooper/internal/ext/component.hpp>
#include <blooper/internal/ext/string.hpp>
#include <blooper/internal/utils/CoreContextFunctions.hpp>
#include <blooper/internal/utils/style.hpp>
#include <blooper/internal/utils/gui.hpp>
#include <blooper/internal/utils/BarComponents.hpp>

BLOOPER_NAMESPACE_BEGIN

[[maybe_unused]] ProjectsMenuComponent::ProjectsMenuComponent(
    AbstractCoreContext& context,
    State                state,
    ProjectsMenuOptions  options)
    : CoreComponentBase(
          context,
          move(state)),
      options(move(options))
{
  auto& manager = this->getContext().getEngine().getProjectManager();
  manager.loadList();


  this->projects = manager.getAllProjects(manager.folders);


  this->isAddingProject.referTo(
      this->getState(),
      isAddingProjectId,
      this->getContext().getUndoManagerPtr(),
      false);

  this->projectPath.referTo(
      this->getState(),
      projectPathId,
      this->getContext().getUndoManagerPtr(),
      "");

  this->projectName.referTo(
      this->getState(),
      projectFileId,
      this->getContext().getUndoManagerPtr(),
      "New project");


  this->list =
      std::make_unique<juce::ListBox>(
          "Projects",
          this);

  this->list->setMultipleSelectionEnabled(false);
  this->list->setRowSelectedOnMouseDown(true);
  this->list->setRowHeight(30);


  this->reloadProjectsButton =
      std::make_unique<juce::TextButton>(
          "Reload");

  this->reloadProjectsButton->onClick = [this] {
    this->getContext().getEngine().getProjectManager().loadList();

    this->reloadProjects();
  };


  this->addProjectButton =
      std::make_unique<juce::TextButton>(
          "Add Project...");

  this->addProjectButton->onClick = [this] {
    if (this->isAddingProject)
    {
      addProject(
          this->getContext(),
          ext::splitPath(*projectPath),
          projectName);

      this->reloadProjects();
    }

    this->toggleAddingProject();
  };


  this->deleteProjectButton =
      std::make_unique<juce::TextButton>(
          "Delete");

  this->deleteProjectButton->onClick = [this] {
    auto row = this->list->getSelectedRow();
    if (!this->isValidRow(row)) return;

    deleteProject(
        this->getContext(),
        projects[row]);

    this->reloadProjects();
  };


  this->cancelButton =
      std::make_unique<juce::TextButton>(
          "Cancel");

  this->cancelButton->onClick = [this] {
    if (!this->isAddingProject)
    {
      this->options.onCancel();
    }
    else
    {
      this->toggleAddingProject();
    }
  };


  this->openProjectButton =
      std::make_unique<juce::TextButton>(
          "Open...");

  this->openProjectButton->onClick = [this] {
    auto row = this->list->getSelectedRow();
    if (!this->isValidRow(row)) return;

    this->options.onOpen(projects[row]);
  };


  this->addProjectPanel =
      std::make_unique<juce::PropertyPanel>(
          "Add project...");

  this->addProjectPanel->addProperties(
      {new juce::TextPropertyComponent(
           projectPath.getPropertyAsValue(),
           "Path",
           200,
           false,
           true),

       new juce::TextPropertyComponent(
           projectName.getPropertyAsValue(),
           "Name",
           200,
           false,
           true)},


      2);


  ext::addAndMakeVisible(
      *this,
      *this->list,

      *this->reloadProjectsButton,
      *this->addProjectButton,
      *this->deleteProjectButton,

      *this->cancelButton,
      *this->openProjectButton,

      *this->addProjectPanel);

  this->addProjectPanel->setVisible(
      false);
}


[[maybe_unused]] bool ProjectsMenuComponent::isValidRow(
    int row) const noexcept
{
  return row >= 0 && row < this->projects.size();
}

[[maybe_unused]] void ProjectsMenuComponent::toggleAddingProject()
{
  if (this->isAddingProject)
  {
    this->isAddingProject = false;
    this->addProjectButton->setButtonText("Add...");
    this->addProjectPanel->setVisible(false);
  }
  else
  {
    this->isAddingProject = true;
    this->addProjectButton->setButtonText("Add");
    this->addProjectPanel->setVisible(true);
  }

  this->resized();
}

[[maybe_unused]] void ProjectsMenuComponent::reloadProjects()
{
  auto& manager = this->getContext().getEngine().getProjectManager();

  this->projects = manager.getAllProjects(manager.folders);

  this->list->updateContent();
}


// Component

void ProjectsMenuComponent::resized()
{
  auto availableArea =
      util::buildBar(
          this->getLookAndFeel(),
          this->getLocalBounds())
          .addComponentsRight(
              *this->openProjectButton,
              *this->cancelButton)
          .addComponentsLeft(
              *this->reloadProjectsButton,
              *this->addProjectButton,
              *this->deleteProjectButton)
          .finish();

  if (this->isAddingProject)
  {
    this->addProjectPanel->setBounds(
        availableArea.removeFromBottom(
            availableArea.getHeight() / 10));
  }

  this->list->setBounds(availableArea);
}


// ListBoxModel

[[maybe_unused]] int ProjectsMenuComponent::getNumRows()
{
  return this->projects.size();
}

[[maybe_unused]] void ProjectsMenuComponent::paintListBoxItem(
    int             row,
    juce::Graphics& g,
    int             width,
    int             height,
    bool            isSelected)
{
  if (!this->isValidRow(row)) return;

  auto project = this->projects[row];
  if (!project) return;


  // Separator

  auto availableArea =
      util::drawBottomLine(
          g,
          *this,
          JuceBounds{
              0,
              0,
              width,
              height},
          this->getContext()
              .getSelectionManager()
              .isSelected(project));


  // Path - TODO: cache paths somehow

  g.setColour(
      this->findColour(
          ColourId::white));

  g.drawText(
      getProjectPath(
          this->getContext(),
          *project)
          .joinIntoString(pathSeparator),
      availableArea.removeFromLeft(50),
      juce::Justification::left);


  // Arrow - TODO: encapsulate?

  g.setColour(
      this->findColour(
          ColourId::purple));

  const auto arrowLineStart =
      availableArea
          .getRelativePoint(
              0.0,
              0.5)
          .toFloat();

  const auto arrowLineEnd =
      arrowLineStart.withX(
          arrowLineStart.getX() + 10);

  g.drawArrow(
      {move(arrowLineStart),
       move(arrowLineEnd)},
      outlineThickness,
      10,
      10);

  availableArea.removeFromLeft(30);


  // Name

  if (isSelected)
  {
    g.setColour(
        this->findColour(
            ColourId::selection));
  }
  else
  {
    g.setColour(
        this->findColour(
            ColourId::white));
  }

  g.drawText(
      project->getName(),
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

  this->options.onOpen(this->projects[row]);
}

[[maybe_unused]] void ProjectsMenuComponent::deleteKeyPressed(
    int row)
{
  if (!this->isValidRow(row)) return;

  deleteProject(
      this->getContext(),
      this->projects[row]);

  this->reloadProjects();
}

BLOOPER_NAMESPACE_END
