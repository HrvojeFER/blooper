#ifndef BLOOPER_RACK_PANEL_COMPONENT_HPP
#define BLOOPER_RACK_PANEL_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

class ControlSurfacePanelComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(ControlSurfacePanelComponent);


  struct Options
  {
  } options;

  explicit ControlSurfacePanelComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});

  ~ControlSurfacePanelComponent() override;


 private:
  juce::OwnedArray<class ControlSurfaceKnobComponent>   knobs;
  juce::OwnedArray<class ControlSurfaceSliderComponent> sliders;


  // Component

 public:
  void paint(JuceGraphics&) override;

  void resized() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ControlSurfacePanelComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_RACK_PANEL_COMPONENT_HPP
