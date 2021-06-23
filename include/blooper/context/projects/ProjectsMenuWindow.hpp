#ifndef BLOOPER_PROJECTS_MENU_WINDOW_HPP
#define BLOOPER_PROJECTS_MENU_WINDOW_HPP

#include <blooper/internal/abstract/windows.hpp>

BLOOPER_NAMESPACE_BEGIN

class ProjectsMenuWindow : public CoreWindowBase
{
 public:
  BLOOPER_STATE_ID(ProjectsMenuWindow);


  struct Options
  {
    std::function<void(JuceProjectRef)> onOpen;
    std::function<void()>               onCancel;
  } options;

  explicit ProjectsMenuWindow(
      AbstractCoreContext& context,
      State                state,
      Options              options = {});


  // Window

 private:
  [[maybe_unused]] void closeButtonPressed() override;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectsMenuWindow)
};


[[maybe_unused]] ProjectsMenuWindow* showProjectsMenu(
    AbstractCoreContext&        context,
    ProjectsMenuWindow::Options options = {});

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PROJECTS_MENU_WINDOW_HPP
