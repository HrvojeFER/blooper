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


ProjectsMenuComponent::ProjectsMenuComponent(
    AbstractCoreContext& context,
    State                state,
    Options              options)
    : CoreComponentBase(
          context,
          std::move(state)),
      options(std::move(options)),

      projects(findProjectsWithFolders(
          context.getEngine().getProjectManager())),

      list("Projects", this),

      reloadProjectsButton("Reload"),
      addProjectButton("Add..."),
      deleteProjectButton("Delete"),

      cancelButton("Cancel"),
      openProjectButton("Open..."),

      projectPathProperty(),
      projectFileProperty(),

      isAddingProject(false),
      addProjectPanel("Add project...")
{
  list.setMultipleSelectionEnabled(false);
  list.setRowSelectedOnMouseDown(true);


  addProjectPanel.addProperties(
      {new juce::TextPropertyComponent(
           projectPathProperty,
           "Folder",
           200,
           false,
           true),

       new juce::TextPropertyComponent(
           projectFileProperty,
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
          projectFileProperty.getValue().toString(),
          ensureProjectPath(
              this->context.getEngine(),
              projectPathProperty.getValue().toString()));

      reloadProjects();

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
  };

  deleteProjectButton.onClick = [this] {
    auto row = list.getSelectedRow();
    if (row == -1) return;

    auto& manager =
        this->getContext()
            .getEngine()
            .getProjectManager();

    manager.deleteProjectFolder(projects[row].folder);

    this->reloadProjects();
    this->repaint();
  };

  cancelButton.onClick = [this] {
    if (this->isAddingProject)
    {
      this->isAddingProject = false;
      this->addProjectButton.setButtonText("Add...");
      this->addProjectPanel.setVisible(false);

#ifdef __JETBRAINS_IDE__
  #pragma clang diagnostic push
  #pragma ide diagnostic   ignored "VirtualCallInCtorOrDtor"
#endif

      this->resized();

#ifdef __JETBRAINS_IDE__
  #pragma clang diagnostic pop
#endif
    }
    else
    {
      this->options.onCancel();
    }
  };

  openProjectButton.onClick = [this] {
    auto row = list.getSelectedRow();
    if (row == -1) return;

    this->options.onOpen(projects[row].project);
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

ProjectsMenuComponent::~ProjectsMenuComponent()
{
  //    auto& manager = engine.getProjectManager();

  //    manager.saveList();
}


void ProjectsMenuComponent::resized()
{
  auto availableArea = this->getLocalBounds();
  auto availableButtonArea =
      availableArea.removeFromBottom(20);

  if (isAddingProject)
  {
    const auto listHeight = int(availableArea.getHeight() * 0.6);
    const auto panelHeight = int(availableArea.getHeight() * 0.4);

    list.setBounds(availableArea.removeFromTop(listHeight));
    addProjectPanel.setBounds(availableArea.removeFromTop(panelHeight));
  }
  else
  {
    list.setBounds(availableArea);
  }


  const auto buttonWidth = 50;

  reloadProjectsButton.setBounds(
      availableButtonArea.removeFromLeft(buttonWidth)
          .reduced(2));
  addProjectButton.setBounds(
      availableButtonArea.removeFromLeft(buttonWidth)
          .reduced(2));
  deleteProjectButton.setBounds(
      availableButtonArea.removeFromLeft(buttonWidth)
          .reduced(2));

  openProjectButton.setBounds(
      availableButtonArea.removeFromRight(buttonWidth)
          .reduced(2));
  cancelButton.setBounds(
      availableButtonArea.removeFromRight(buttonWidth)
          .reduced(2));
}


int ProjectsMenuComponent::getNumRows()
{
  return projects.size();
}

void ProjectsMenuComponent::paintListBoxItem(
    int             rowNumber,
    juce::Graphics& g,
    int             width,
    int             height,
    bool            rowIsSelected)
{
  if (rowNumber < 0 || rowNumber >= projects.size()) return;

  auto   project = projects[rowNumber];
  auto&& projectName = project.project->getName();
  auto&& projectPath = project.path;


  auto rectangle = juce::Rectangle<int>(0, 0, width, height);
  auto subRowHeight = height / 2;


  if (rowIsSelected)
    g.setColour(juce::Colours::lightgrey);
  else
    g.setColour(juce::Colours::white);

  g.drawText(projectName,
             rectangle.removeFromTop(subRowHeight).reduced(2),
             juce::Justification::left);


  g.setColour(juce::Colours::whitesmoke);

  g.drawText(projectPath,
             rectangle.removeFromTop(subRowHeight).reduced(2),
             juce::Justification::left);
}


void ProjectsMenuComponent::listBoxItemClicked(
    int                     row,
    const juce::MouseEvent& event)
{
  ListBoxModel::listBoxItemClicked(row, event);
}

void ProjectsMenuComponent::listBoxItemDoubleClicked(
    int row,
    const juce::MouseEvent&)
{
  if (row == -1) return;
  this->options.onOpen(projects[row].project);
}

void ProjectsMenuComponent::deleteKeyPressed(
    int lastRowSelected)
{
  auto& manager = this->getContext().getEngine().getProjectManager();

  manager.deleteProjectFolder(projects[lastRowSelected].folder);
  projects = findProjectsWithFolders(manager.folders);

  this->repaint();
}


void ProjectsMenuComponent::reloadProjects()
{
  auto& manager = this->getContext().getEngine().getProjectManager();

  manager.saveList();
  manager.loadList();
  projects = findProjectsWithFolders(manager);
}


auto getFolderProject(const juce::ValueTree& folder)
{
  return dynamic_cast<te::Project*>(
      folder.getProperty(te::IDs::project).getObject());
}

auto getFolderName(const juce::ValueTree& folder)
{
  return folder.getProperty(te::IDs::name).toString();
}

ProjectsMenuComponent::ProjectArray
// NOLINTNEXTLINE(misc-no-recursion)
ProjectsMenuComponent::findProjectsWithFolders(
    const juce::ValueTree& folder,
    const juce::String&    path)
{
  ProjectsMenuComponent::ProjectArray result;

  const auto currentPath =
      path.isEmpty() ?
          getFolderName(folder) :
          (path + ext::pathSeparator + getFolderName(folder));

  if (auto project = getFolderProject(folder))
    result.add({currentPath, folder, project});

  for (int i = 0; i < folder.getNumChildren(); ++i)
    result.addArray(findProjectsWithFolders(
        folder.getChild(i),
        currentPath));

  return result;
}

ProjectsMenuComponent::ProjectArray
ProjectsMenuComponent::findProjectsWithFolders(
    te::ProjectManager& manager)
{
  manager.loadList();
  return findProjectsWithFolders(manager.folders);
}

BLOOPER_NAMESPACE_END
