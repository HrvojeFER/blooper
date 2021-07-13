#include <blooper/components/takes/RecordingTakeComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

RecordingTakeComponent::RecordingTakeComponent(
    AbstractContext&              context,
    State                         state,
    RecordingTakeComponentOptions options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}

RecordingTakeComponent::~RecordingTakeComponent() = default;


// Component

void RecordingTakeComponent::resized()
{
}

BLOOPER_NAMESPACE_END
