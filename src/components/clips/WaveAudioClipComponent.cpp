#include <blooper/components/clips/WaveAudioClipComponent.hpp>

#include <blooper/internal/ext/component.hpp>

#include <blooper/components/takes/WaveAudioTakeComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

WaveAudioClipComponent::WaveAudioClipComponent(
    AbstractContext&              context,
    State                         state,
    JuceWaveAudioClipRef          clip,
    WaveAudioClipComponentOptions options)
    : WaveAudioClipComponent::base(
    context,
    move(state),
    move(clip)),
      options(move(options))
{
  this->updateCurrentTake();
}

WaveAudioClipComponent::~WaveAudioClipComponent() = default;


void WaveAudioClipComponent::updateCurrentTake()
{
  auto currentTakeIndex = this->getClip().getCurrentTake();
  if (currentTakeIndex ==
      this->currentTakeComponent->getTakeRef().getIndex())
    return;

  auto takesIds = this->getClip().getTakes();
  auto currentTakeId = takesIds[currentTakeIndex];

  WaveAudioTakeRef currentTake{
      std::addressof(this->getClip()),
      currentTakeIndex,
      currentTakeId};

  this->removeChildComponent(this->currentTakeComponent.get());
  this->currentTakeComponent =
      addChild<WaveAudioTakeComponent>(
          *this,
          move(currentTake));
}


// Component

void WaveAudioClipComponent::resized()
{
  auto availableArea = this->getLocalBounds();

  if (this->currentTakeComponent)
    this->currentTakeComponent->setBounds(availableArea);
}

BLOOPER_NAMESPACE_END
