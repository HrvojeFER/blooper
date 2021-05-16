#ifndef BLOOPER_PROJECTS_MENU_COMPONENT_HPP
#define BLOOPER_PROJECTS_MENU_COMPONENT_HPP


#include <blooper/internal/internal.hpp>


BLOOPER_NAMESPACE_BEGIN

class ProjectsMenuComponent :
    public juce::Component,
    private juce::ListBoxModel
{
public:
    explicit ProjectsMenuComponent(te::Engine& engine);
    ~ProjectsMenuComponent() override;


    using ProjectRef = te::Project::Ptr;
    std::function<void(ProjectRef)> onOpen;

    std::function<void()> onCancel;


    void resized() override;


private:
    te::Engine& engine;


    struct ProjectWithPath
    {
        juce::String     path;
        juce::ValueTree  folder;
        te::Project::Ptr project;
    };

    using ProjectArray = juce::Array<ProjectWithPath>;
    ProjectArray projects;

    friend class juce::ListBox;
    juce::ListBox list;


    juce::TextButton
            reloadProjectsButton,
            addProjectButton,
            deleteProjectButton,
            cancelButton,
            openProjectButton;


    bool isAddingProject;

    juce::Value
            projectPathProperty,
            projectFileProperty;

    juce::PropertyPanel addProjectPanel;


    int getNumRows() override;

    void paintListBoxItem(
            int             rowNumber,
            juce::Graphics& g,
            int             width,
            int             height,
            bool            rowIsSelected) override;


    void listBoxItemClicked(
            int                     row,
            const juce::MouseEvent& event) override;

    void listBoxItemDoubleClicked(
            int                     row,
            const juce::MouseEvent& event) override;

    void deleteKeyPressed(int lastRowSelected) override;


    void reloadProjects();


    static ProjectArray findProjectsWithFolders(
            const juce::ValueTree& folder,
            const juce::String&    path = {});

    static ProjectArray findProjectsWithFolders(
            te::ProjectManager& manager);
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_PROJECTS_MENU_COMPONENT_HPP
