#ifndef BLOOPER_PROJECTS_MENU_COMPONENT_HPP
#define BLOOPER_PROJECTS_MENU_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class ProjectsMenuComponent :
    public CoreComponentBase,
    private juce::ListBoxModel
{
 public:
  BLOOPER_STATE_ID(ProjectsMenuComponent);


  struct Options
  {
    std::function<void(JuceProjectRef)> onOpen;
    std::function<void()>               onCancel;
  } options;

  explicit ProjectsMenuComponent(
      AbstractCoreContext& context,
      State                state,
      Options              options = {});

  ~ProjectsMenuComponent() override;


  void resized() override;


 private:
  struct ProjectWithPath
  {
    juce::String    path;
    juce::ValueTree folder;
    JuceProjectRef  project;
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
