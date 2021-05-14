#include <blooper/context/projects/ProjectsMenuComponent.hpp>


BLOOPER_NAMESPACE_BEGIN

ProjectsMenuComponent::ProjectsMenuComponent(te::Engine& engine)
    : engine(engine),
      projects(findProjectsWithFolders(engine.getProjectManager())),

      list("Projects", this),

      reloadProjectsButton("Reload"),
      addProjectButton("Add..."),
      deleteProjectButton("Delete"),

      cancelButton("Cancel"),
      openProjectButton("Open..."),

      isAddingProject(false),
      addProjectPanel("Add project...")
{
    list.setMultipleSelectionEnabled(false);
    list.setRowSelectedOnMouseDown(true);


    addProjectPanel.addProperties(
            {new juce::TextPropertyComponent(
                     folderNameProperty,
                     "Folder",
                     50,
                     false,
                     true),

             new juce::TextPropertyComponent(
                     projectNameProperty,
                     "Name",
                     50,
                     false,
                     true)},

            2);


    reloadProjectsButton.onClick = [this] {
        this->reloadProjects();
    };

    addProjectButton.onClick = [this] {
        if (!this->isAddingProject)
        {
            this->isAddingProject = true;
            this->addProjectButton.setName("Add");

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
        else
        {
            auto& manager = this->engine.getProjectManager();

            manager.createNewProjectInteractively()
        }
    };

    deleteProjectButton.onClick = [this] {
        auto row = list.getSelectedRow();
        if (row == -1) return;

        auto& manager = this->engine.getProjectManager();

        manager.deleteProjectFolder(projects[row].folder);

        this->projects = findProjectsWithFolders(manager.folders);
        this->repaint();
    };


    cancelButton.onClick = [this] {

    };

    openProjectButton.onClick = [this] {
        auto row = list.getSelectedRow();
        if (row == -1) return;

        this->onOpen(projects[row].project);
    };


    utils::addAndMakeVisible(
            *this,
            {&list,

             &reloadProjectsButton,
             &addProjectButton,
             &deleteProjectButton,

             &cancelButton,
             &openProjectButton,

             &addProjectPanel});
}

ProjectsMenuComponent::~ProjectsMenuComponent()
{
    auto& manager = engine.getProjectManager();

    manager.saveList();
}


void ProjectsMenuComponent::paint(juce::Graphics& g)
{
    g.fillAll();
}

void ProjectsMenuComponent::resized()
{
    auto availableArea = getLocalBounds();

    auto listHeight = int(availableArea.getHeight() * 0.9);
    list.setBounds(availableArea.removeFromTop(listHeight));

    const auto buttonWidth = 20;

    reloadProjectsButton.setBounds(
            availableArea.removeFromLeft(buttonWidth)
                    .reduced(2));
    addProjectButton.setBounds(
            availableArea.removeFromLeft(buttonWidth)
                    .reduced(2));
    deleteProjectButton.setBounds(
            availableArea.removeFromLeft(buttonWidth)
                    .reduced(2));

    openProjectButton.setBounds(
            availableArea.removeFromRight(buttonWidth)
                    .reduced(2));
    cancelButton.setBounds(
            availableArea.removeFromRight(buttonWidth)
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
    auto&& folderName = project.folder.getProperty(te::IDs::name).toString();


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

    g.drawText(folderName,
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
    onOpen(projects[row].project);
}

void ProjectsMenuComponent::deleteKeyPressed(int lastRowSelected)
{
    auto& manager = engine.getProjectManager();

    manager.deleteProjectFolder(projects[lastRowSelected].folder);
    projects = findProjectsWithFolders(manager.folders);

    repaint();
}


void ProjectsMenuComponent::reloadProjects()
{
    auto& manager = engine.getProjectManager();

    manager.saveList();
    projects = findProjectsWithFolders(manager);

    repaint();
}


auto getProject(const juce::ValueTree& folder)
{
    return dynamic_cast<te::Project*>(
            folder.getProperty(te::IDs::project).getObject());
}

ProjectsMenuComponent::ProjectArray
ProjectsMenuComponent::findProjectsWithFolders(const juce::ValueTree& root)
{
    ProjectsMenuComponent::ProjectArray result;

    if (auto project = getProject(root))
        result.add({root, project});

    for (int i = 0; i < root.getNumChildren(); ++i)
        result.addArray(findProjectsWithFolders(
                root.getChild(i)));

    return result;
}

ProjectsMenuComponent::ProjectArray
ProjectsMenuComponent::findProjectsWithFolders(te::ProjectManager& manager)
{
    manager.loadList();
    return findProjectsWithFolders(manager.folders);
}

BLOOPER_NAMESPACE_END
