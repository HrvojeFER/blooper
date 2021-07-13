#include <blooper/components/clips/AudioClipComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

AudioClipComponent::AudioClipComponent(
    AbstractContext&          context,
    State                     state,
    AudioClipComponentOptions options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}

AudioClipComponent::~AudioClipComponent() = default;


// Component

void AudioClipComponent::resized()
{
}

BLOOPER_NAMESPACE_END
