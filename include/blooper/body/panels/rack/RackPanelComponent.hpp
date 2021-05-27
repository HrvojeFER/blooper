#ifndef BLOOPER_RACK_PANEL_COMPONENT_HPP
#define BLOOPER_RACK_PANEL_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class RackPanelComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(RackPanelComponent);


  struct Options
  {
  } options;

  explicit RackPanelComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});


  void resized() override;


 private:
  juce::OwnedArray<class RackKnobComponent>   knobs;
  juce::OwnedArray<class RackSliderComponent> sliders;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RackPanelComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_RACK_PANEL_COMPONENT_HPP
