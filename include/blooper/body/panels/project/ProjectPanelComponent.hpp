#ifndef BLOOPER_PROJECT_PANEL_COMPONENT_HPP
#define BLOOPER_PROJECT_PANEL_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class ProjectPanelComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(ProjectPanelComponent);

  BLOOPER_ID(editScrollRangeStart);
  BLOOPER_ID(editScrollRangeEnd);


  struct Options
  {
  } options;

  explicit ProjectPanelComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});

  ~ProjectPanelComponent() override;


  void resized() override;


 private:
  std::unique_ptr<class ProjectContentComponent> content;

  std::unique_ptr<juce::Viewport> viewport;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectPanelComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PROJECT_PANEL_COMPONENT_HPP
