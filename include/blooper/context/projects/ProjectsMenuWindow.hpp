#ifndef BLOOPER_PROJECTS_MENU_WINDOW_HPP
#define BLOOPER_PROJECTS_MENU_WINDOW_HPP


#include <blooper/fwd.hpp>

#include <blooper/context/core/core.hpp>


BLOOPER_NAMESPACE_BEGIN

class ProjectsMenuWindow : public CoreWindow
{
public:
    explicit ProjectsMenuWindow(CoreContext& context);
    ~ProjectsMenuWindow() override;


    std::function<void(ProjectsMenuComponent::ProjectRef)> onOpen;

    std::function<void()> onCancel;


    void closeButtonPressed() override;


private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectsMenuWindow);
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_PROJECTS_MENU_WINDOW_HPP
