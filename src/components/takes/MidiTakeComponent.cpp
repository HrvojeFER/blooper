#include <blooper/components/takes/MidiTakeComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

MidiTakeComponent::MidiTakeComponent(
    AbstractContext&         context,
    State                    state,
    MidiTakeComponentOptions options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}

MidiTakeComponent::~MidiTakeComponent() = default;


// Component

void MidiTakeComponent::resized()
{
}

BLOOPER_NAMESPACE_END
