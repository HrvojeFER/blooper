#include <blooper/components/clips/MidiClipComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

MidiClipComponent::MidiClipComponent(
    AbstractContext&         context,
    State                    state,
    MidiClipComponentOptions options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}

MidiClipComponent::~MidiClipComponent() = default;


// Component

void MidiClipComponent::resized()
{
}

BLOOPER_NAMESPACE_END
