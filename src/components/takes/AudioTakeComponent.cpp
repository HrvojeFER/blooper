#include <blooper/components/takes/AudioTakeComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

AudioTakeComponent::AudioTakeComponent(
    AbstractContext&         context,
    State                    state,
    AudioTakeComponentOptions options)
    : ComponentBase(
    context,
    move(state)),
      options(move(options))
{
}

AudioTakeComponent::~AudioTakeComponent() = default;


// Component

void AudioTakeComponent::resized()
{
}

BLOOPER_NAMESPACE_END
