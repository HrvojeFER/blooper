#ifndef BLOOPER_PROJECTS_MENU_WINDOW_HPP
#define BLOOPER_PROJECTS_MENU_WINDOW_HPP


#include <blooper/internal/internal.hpp>
#include <blooper/style/style.hpp>

#include <blooper/context/projects/ProjectsMenuComponent.hpp>


BLOOPER_NAMESPACE_BEGIN

class ProjectsMenuWindow : juce::DocumentWindow
{
public:
    ProjectsMenuWindow(te::Engine& engine);
    ~ProjectsMenuWindow() override;


private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectsMenuWindow);
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_PROJECTS_MENU_WINDOW_HPP
