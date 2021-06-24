#include <blooper/body/panels/control_surface/ControlSurfacePanelComponent.hpp>

#include <blooper/body/panels/control_surface/ControlSurfaceKnobComponent.hpp>
#include <blooper/body/panels/control_surface/ControlSurfaceSliderComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

ControlSurfacePanelComponent::ControlSurfacePanelComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}

ControlSurfacePanelComponent::~ControlSurfacePanelComponent() = default;


// Component

void ControlSurfacePanelComponent::paint(JuceGraphics& g)
{
  g.setColour(juce::Colours::whitesmoke);

  g.drawRect(
      this->getLocalBounds().reduced(2),
      2);
}

void ControlSurfacePanelComponent::resized()
{
  auto availableArea = this->getLocalBounds().reduced(6);

  maybeUnused(availableArea);
}

BLOOPER_NAMESPACE_END
