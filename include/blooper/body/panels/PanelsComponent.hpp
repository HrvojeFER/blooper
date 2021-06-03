#ifndef BLOOPER_PANELS_COMPONENT_HPP
#define BLOOPER_PANELS_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class PanelsComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(PanelsComponent);


  struct Options
  {
  } options;

  explicit PanelsComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});


  void resized() override;


 private:
  std::unique_ptr<class EditPanelComponent>           editPanel;
  std::unique_ptr<class ControlSurfacePanelComponent> controlSurfacePanel;

  std::unique_ptr<class MasterPanelComponent> masterPanel;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PanelsComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PANELS_COMPONENT_HPP
