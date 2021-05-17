#ifndef BLOOPER_PROJECTS_MENU_COMPONENT_HPP
#define BLOOPER_PROJECTS_MENU_COMPONENT_HPP


#include <blooper/fwd.hpp>

#include <blooper/context/core/core.hpp>


BLOOPER_NAMESPACE_BEGIN

class ProjectsMenuComponent :
    public CoreComponent,
    private juce::ListBoxModel
{
public:
    explicit ProjectsMenuComponent(CoreContext& context);
    ~ProjectsMenuComponent() override;

    using ProjectRef = te::Project::Ptr;
    std::function<void(ProjectRef)> onOpen;

    std::function<void()> onCancel;


    void resized() override;


private:
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


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectsMenuComponent);
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_PROJECTS_MENU_COMPONENT_HPP
