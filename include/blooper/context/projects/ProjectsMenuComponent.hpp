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

  BLOOPER_ID(isAddingProjectId);
  BLOOPER_ID(projectPathId);
  BLOOPER_ID(projectFileId);


  struct Options
  {
    std::function<void(JuceProjectRef)> onOpen;
    std::function<void()>               onCancel;
  } options;

  [[maybe_unused]] explicit ProjectsMenuComponent(
      AbstractCoreContext& context,
      State                state,
      Options              options = {});


  void resized() override;


 private:
  juce::CachedValue<bool> isAddingProject;

  juce::CachedValue<JuceString>
      projectPath,
      projectFile;


  using ProjectArray = juce::ReferenceCountedArray<te::Project>;
  ProjectArray projects;

  friend class juce::ListBox;
  juce::ListBox list;


  juce::TextButton
      reloadProjectsButton,
      addProjectButton,
      deleteProjectButton,
      cancelButton,
      openProjectButton;


  juce::PropertyPanel addProjectPanel;


  [[maybe_unused]] void toggleAddingProject();

  [[maybe_unused]] void reloadProjects();

  [[maybe_unused]] bool isValidRow(int row) const noexcept;


  [[maybe_unused]] int getNumRows() override;

  [[maybe_unused]] void paintListBoxItem(
      int             row,
      juce::Graphics& g,
      int             width,
      int             height,
      bool            rowIsSelected) override;


  [[maybe_unused]] void listBoxItemClicked(
      int                     row,
      const juce::MouseEvent& event) override;

  [[maybe_unused]] void listBoxItemDoubleClicked(
      int                     row,
      const juce::MouseEvent& event) override;

  [[maybe_unused]] void deleteKeyPressed(int lastRowSelected) override;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectsMenuComponent);
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PROJECTS_MENU_COMPONENT_HPP
