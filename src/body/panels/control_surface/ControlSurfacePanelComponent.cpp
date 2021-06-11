#include <blooper/blooper.hpp>

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
