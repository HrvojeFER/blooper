#ifndef BLOOPER_PROJECT_PANEL_COMPONENT_HPP
#define BLOOPER_PROJECT_PANEL_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

class ProjectPanelComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(ProjectPanelComponent);


  struct Options
  {
  } options;

  explicit ProjectPanelComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});

  ~ProjectPanelComponent() override;


 private:
  BLOOPER_ID(projectScrollStartId);
  JuceCached<double> projectScrollStart;

  BLOOPER_ID(projectScrollEndId);
  JuceCached<double> projectScrollEnd;


  std::unique_ptr<class ProjectContentComponent> content;

  std::unique_ptr<juce::Viewport> viewport;


  // Component

 public:
  void paint(JuceGraphics&) override;

  void resized() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectPanelComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PROJECT_PANEL_COMPONENT_HPP
