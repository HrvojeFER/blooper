#include <blooper/body/panels/master/MasterPanelComponent.hpp>

#include <blooper/internal/abstract/const.hpp>
#include <blooper/internal/utils/gui.hpp>

#include <blooper/body/panels/master/MasterTrackComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

MasterPanelComponent::MasterPanelComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}

MasterPanelComponent::~MasterPanelComponent() = default;


// Component

void MasterPanelComponent::paint(JuceGraphics& g)
{
  util::drawOutline(
      g,
      *this);
}

void MasterPanelComponent::resized()
{
  auto availableArea =
      util::pad(
          this->getLocalBounds(),
          outlinePaddingFactor);

  maybeUnused(availableArea);
}

BLOOPER_NAMESPACE_END
