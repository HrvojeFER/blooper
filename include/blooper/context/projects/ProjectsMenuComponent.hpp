#ifndef BLOOPER_PROJECTS_MENU_COMPONENT_HPP
#define BLOOPER_PROJECTS_MENU_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

struct ProjectsMenuOptions
{
  std::function<void(JuceProjectRef)> onOpen{[](auto) {}};
  std::function<void()>               onCancel{[] {}};
};

class ProjectsMenuComponent :
    public CoreComponentBase,
    public juce::ListBoxModel
{
 public:
  BLOOPER_STATE_ID(ProjectsMenuComponent);

  [[maybe_unused]] explicit ProjectsMenuComponent(
      AbstractCoreContext& context,
      State                state,
      ProjectsMenuOptions  options = {});

  ProjectsMenuOptions options;

  BLOOPER_ID(isAddingProjectId);
  juce::CachedValue<bool> isAddingProject;

  BLOOPER_ID(projectPathId);
  juce::CachedValue<JuceString> projectPath;

  BLOOPER_ID(projectFileId);
  juce::CachedValue<JuceString> projectName;


 private:
  std::unique_ptr<juce::TextButton>
      reloadProjectsButton,
      addProjectButton,
      deleteProjectButton,
      cancelButton,
      openProjectButton;

  std::unique_ptr<juce::PropertyPanel>
      addProjectPanel;

  std::unique_ptr<juce::ListBox>
      list;


  using ProjectArray = juce::ReferenceCountedArray<te::Project>;
  ProjectArray projects;


  [[maybe_unused, nodiscard]] bool isValidRow(
      int row) const noexcept;

  [[maybe_unused]] void toggleAddingProject();

  [[maybe_unused]] void reloadProjects();


  // Component

 public:
  void resized() override;


  // ListBoxModel

 public:
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

  [[maybe_unused]] void deleteKeyPressed(
      int row) override;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectsMenuComponent);
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PROJECTS_MENU_COMPONENT_HPP
