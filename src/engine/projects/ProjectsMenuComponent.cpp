#include <blooper/engine/projects/ProjectsMenuComponent.hpp>


BLOOPER_NAMESPACE_BEGIN

ProjectsMenuComponent::ProjectsMenuComponent(te::Engine& engine)
    : engine(engine),
      projects(findProjectsWithFolders(engine.getProjectManager())),
      list("Projects", this),
      addProjectButton("Add..."),
      deleteProjectButton("Delete"),
      openProjectButton("Open...")
{
    list.setMultipleSelectionEnabled(false);
    list.setRowSelectedOnMouseDown(true);

    addProjectButton.onClick = [this] {
        // TODO

        this->engine.getProjectManager().saveList();
        this->projects =
                findProjectsWithFolders(
                        this->engine.getProjectManager().folders);
        repaint();
    };

    deleteProjectButton.onClick = [this] {
        auto row = list.getSelectedRow();
        if (row == -1) return;

        this->engine.getProjectManager().deleteProjectFolder(
                projects[row].folder);

        this->engine.getProjectManager().saveList();
        this->projects =
                findProjectsWithFolders(
                        this->engine.getProjectManager().folders);
        repaint();
    };

    openProjectButton.onClick = [this] {
        auto row = list.getSelectedRow();
        if (row == -1) return;

        this->onOpen(projects[row].project);
    };

    utils::addAndMakeVisible(
            *this,
            {&list,
             &addProjectButton,
             &deleteProjectButton,
             &openProjectButton});
}


void ProjectsMenuComponent::paint(juce::Graphics& g)
{
    g.fillAll();
}

void ProjectsMenuComponent::resized()
{
    auto rectangle = getLocalBounds();

    auto listHeight = int(rectangle.getHeight() * 0.9);
    list.setBounds(rectangle.removeFromTop(listHeight));

    auto rightLowRectangle = rectangle;
    auto buttonWidth = 20;
    addProjectButton.setBounds(
            rightLowRectangle.removeFromLeft(buttonWidth)
                    .reduced(2));
    deleteProjectButton.setBounds(
            rightLowRectangle.removeFromLeft(buttonWidth)
                    .reduced(2));

    auto leftLowRectangle = rectangle;
    openProjectButton.setBounds(
            leftLowRectangle.removeFromRight(buttonWidth)
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
    this->engine.getProjectManager().deleteProjectFolder(
            projects[lastRowSelected].folder);
}


auto getProject(const juce::ValueTree& folder)
{
    return dynamic_cast<te::Project*>(
            folder.getProperty(te::IDs::project).getObject());
}

ProjectsMenuComponent::ProjectArray findProjectsWithFolders(
        const juce::ValueTree& root)
{
    ProjectsMenuComponent::ProjectArray result;

    if (auto project = getProject(root))
        result.add({root, project});

    for (int i = 0; i < root.getNumChildren(); ++i)
        result.addArray(findProjectsWithFolders(
                root.getChild(i)));

    return result;
}

ProjectsMenuComponent::ProjectArray findProjectsWithFolders(
        te::ProjectManager& manager)
{
    manager.loadList();
    return findProjectsWithFolders(manager.folders);
}

BLOOPER_NAMESPACE_END
