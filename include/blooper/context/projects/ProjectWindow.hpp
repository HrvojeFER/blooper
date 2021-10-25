#ifndef BLOOPER_PROJECT_WINDOW_HPP
#define BLOOPER_PROJECT_WINDOW_HPP
#pragma once

#include <blooper/internal/abstract/windows.hpp>

BLOOPER_NAMESPACE_BEGIN

class ProjectWindow : public WindowBase
{
 public:
  BLOOPER_STATE_ID(ProjectWindow);


  struct Options
  {
  } options;

  explicit ProjectWindow(
      AbstractContext& context,
      State            state,
      JuceProjectRef   project = {},
      Options          options = {});


 private:
  JuceProjectRef project;


  // Window

 private:
  [[maybe_unused]] void closeButtonPressed() override;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectWindow);
};


[[maybe_unused]] ProjectWindow* showProject(
    AbstractContext&       context,
    JuceProjectRef         project,
    ProjectWindow::Options options = {});

BLOOPER_NAMESPACE_END

#endif //BLOOPER_PROJECT_WINDOW_HPP
