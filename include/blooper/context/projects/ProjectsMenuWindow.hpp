#ifndef BLOOPER_PROJECTS_MENU_WINDOW_HPP
#define BLOOPER_PROJECTS_MENU_WINDOW_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class ProjectsMenuWindow : public CoreWindowBase
{
 public:
  BLOOPER_STATE_ID(ProjectsMenuWindow);

  using ProjectRef = te::Project::Ptr;


  struct Options
  {
    std::function<void(ProjectRef)> onOpen;

    std::function<void()> onCancel;
  } options;

  explicit ProjectsMenuWindow(
      AbstractCoreContext& context,
      State                state,
      Options              options = {});


  void closeButtonPressed() override;


 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectsMenuWindow)
};


[[maybe_unused]] void showProjectsMenu(
    AbstractCoreContext&        context,
    ProjectsMenuWindow::Options options = {});

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PROJECTS_MENU_WINDOW_HPP
