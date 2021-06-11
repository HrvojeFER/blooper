#ifndef BLOOPER_MASTER_PANEL_COMPONENT_HPP
#define BLOOPER_MASTER_PANEL_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class MasterPanelComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(MasterPanelComponent);


  struct Options
  {
  } options;

  explicit MasterPanelComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});


  void paint(JuceGraphics&) override;

  void resized() override;


 private:
  std::unique_ptr<class MasterTrackComponent> track;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MasterPanelComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_MASTER_PANEL_COMPONENT_HPP
