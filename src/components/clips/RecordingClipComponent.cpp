#include <blooper/components/clips/RecordingClipComponent.hpp>

#include <blooper/components/takes/RecordingTakeComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

RecordingClipComponent::RecordingClipComponent(
    AbstractContext&              context,
    State                         state,
    AudioTrackRef                 track,
    RecordingClipComponentOptions options)
    : RecordingClipComponent::base(
          context,
          move(state),
          move(track)),
      options(move(options))
{
  this->takeComponent =
      addChild<RecordingTakeComponent>(
          *this,
          this->getHeldTrackRef());
}

RecordingClipComponent::~RecordingClipComponent() = default;


// Component

void RecordingClipComponent::resized()
{
  auto availableArea = this->getLocalBounds();

  this->takeComponent->setBounds(availableArea);
}

BLOOPER_NAMESPACE_END
