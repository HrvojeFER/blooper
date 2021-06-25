#include <blooper/body/panels/control_surface/ControlSurfacePanelComponent.hpp>

#include <blooper/internal/abstract/const.hpp>
#include <blooper/internal/utils/gui.hpp>

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
  util::drawOutline(
      g,
      *this);
}

void ControlSurfacePanelComponent::resized()
{
  auto availableArea =
      util::pad(
          this->getLocalBounds(),
          outlinePaddingFactor);

  maybeUnused(availableArea);
}

BLOOPER_NAMESPACE_END
