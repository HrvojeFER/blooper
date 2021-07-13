#include <blooper/components/clips/RecordingClipComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

RecordingClipComponent::RecordingClipComponent(
    AbstractContext&              context,
    State                         state,
    RecordingClipComponentOptions options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}

RecordingClipComponent::~RecordingClipComponent() = default;


// Component

void RecordingClipComponent::resized()
{
}

BLOOPER_NAMESPACE_END
