#include <blooper/body/panels/master/MasterTrackComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

MasterTrackComponent::MasterTrackComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}

MasterTrackComponent::~MasterTrackComponent() = default;


// Component

void MasterTrackComponent::resized()
{
}

BLOOPER_NAMESPACE_END
