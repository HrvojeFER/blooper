#include <blooper/components/clips/MidiClipComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

MidiClipComponent::MidiClipComponent(
    AbstractContext&         context,
    State                    state,
    MidiClipRef              clip,
    MidiClipComponentOptions options)
    : MidiClipComponent::base(
          context,
          move(state),
          move(clip)),
      options(move(options))
{
  this->updateCurrentTake();
}

MidiClipComponent::~MidiClipComponent() = default;


void MidiClipComponent::updateCurrentTake()
{
  auto currentTakeIndex = this->getClip().getCurrentTake();
  if (currentTakeIndex ==
      this->currentTakeComponent->getTakeRef().getIndex())
    return;

  auto takesIds = this->getClip().getTakes();
  auto currentTakeId = takesIds[currentTakeIndex];

  MidiTakeRef currentTake{
      std::addressof(this->getClip()),
      currentTakeIndex,
      currentTakeId};

  this->removeChildComponent(this->currentTakeComponent.get());
  this->currentTakeComponent =
      addChild<MidiTakeComponent>(
          *this,
          move(currentTake));
}


// Component

void MidiClipComponent::resized()
{
  auto availableArea = this->getLocalBounds();

  if (this->currentTakeComponent)
    this->currentTakeComponent->setBounds(availableArea);
}

BLOOPER_NAMESPACE_END
