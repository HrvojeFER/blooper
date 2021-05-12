#ifndef BLOOPER_PROJECT_WINDOW_HPP
#define BLOOPER_PROJECT_WINDOW_HPP


#include <blooper/internal/internal.hpp>


BLOOPER_NAMESPACE_BEGIN

class ProjectMenu :
    public juce::Component,
    private juce::ListBoxModel
{
public:
    explicit ProjectMenu(te::Engine& engine);

    using ProjectRef = te::Project::Ptr;
    std::function<void(ProjectRef)> onOpen;


    void paint(juce::Graphics& g) override;

    void resized() override;


private:
    te::Engine& engine;

    struct ProjectWithFolder
    {
        juce::ValueTree  folder;
        te::Project::Ptr project;
    };

    using ProjectArray = juce::Array<ProjectWithFolder>;
    ProjectArray projects;

    friend class juce::ListBox;
    juce::ListBox list;


    juce::TextButton
            addProjectButton{"Add..."},
            deleteProjectButton{"Delete"},
            openProjectButton{"Open..."};


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


    friend ProjectArray findProjectsWithFolders(const juce::ValueTree& root);
    friend ProjectArray findProjectsWithFolders(te::ProjectManager& manager);
};


static ProjectMenu::ProjectArray findProjectsWithFolders(
        const juce::ValueTree& root);

static ProjectMenu::ProjectArray findProjectsWithFolders(
        te::ProjectManager& manager);


template<
        typename TOnOpen,
        decltype(std::declval<TOnOpen>()(
                std::declval<ProjectMenu::ProjectRef>()))* = nullptr>
[[maybe_unused]] void showProjectMenu(
        te::Engine& engine,
        TOnOpen     onOpen)
{
    juce::DialogWindow::LaunchOptions options;
    options.dialogTitle = TRANS("Projects");
    options.dialogBackgroundColour = juce::Colours::black;
    options.escapeKeyTriggersCloseButton = true;
    options.useNativeTitleBar = true;
    options.resizable = true;
    options.useBottomRightCornerResizer = true;

    auto projectMenu = new ProjectMenu(engine);
    projectMenu->setSize(800, 600);
    options.content.setOwned(projectMenu);
    auto window = options.launchAsync();

    projectMenu->onOpen = [=](auto&& project) {
        onOpen(std::forward<decltype(project)>(project));
        window->~DialogWindow();
    };
}

BLOOPER_NAMESPACE_END


#endif // BLOOPER_PROJECT_WINDOW_HPP
