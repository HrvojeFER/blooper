#ifndef BLOOPER_PROJECT_PANEL_COMPONENT_HPP
#define BLOOPER_PROJECT_PANEL_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

struct ProjectPanelOptions
{
};

class ProjectPanelComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(ProjectPanelComponent);


  explicit ProjectPanelComponent(
      AbstractContext&    context,
      State               state,
      ProjectPanelOptions options = {});

  ~ProjectPanelComponent() override;

  ProjectPanelOptions options;


 private:
  std::unique_ptr<class ProjectContentComponent> content;


  // Component

 public:
  void paint(JuceGraphics& g) override;

  void resized() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectPanelComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PROJECT_PANEL_COMPONENT_HPP
