#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

EditContentComponent::EditContentComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
  this->tracks = juce::OwnedArray<EditTrackComponent>();
}


void EditContentComponent::resized()
{
  auto availableArea = this->getLocalBounds();
  auto trackWidth = availableArea.getWidth() / this->tracks.size();

  for (auto track : this->tracks)
    track->setBounds(
        availableArea.removeFromLeft(
            trackWidth));
}


[[maybe_unused]] bool EditContentComponent::isValid(int index)
{
  return index >= 0 && index < tracks.size();
}

[[maybe_unused]] void EditContentComponent::addTrack(int at)
{
  EditTrackComponent::Options componentOptions;

  const auto track =
      new EditTrackComponent(
          this->getContext(),
          this->getState().getOrCreateChildWithName(
              EditTrackComponent::stateId,
              nullptr),
          move(componentOptions));

  if (this->isValid(at))
    this->tracks.insert(at, track);
  else
    this->tracks.add(track);

  this->resizeToFitTracks();
}

[[maybe_unused]] void EditContentComponent::removeTrack(int at)
{
  if (this->isValid(at))
    this->tracks.remove(at);
  else
    this->tracks.remove(this->tracks.size() - 1);

  this->resizeToFitTracks();
}

[[maybe_unused]] void EditContentComponent::resizeToFitTracks()
{
  auto& settings = this->getContext().getSettings();

  int trackSize = settings.getProperty(id::trackSize);
  if (trackSize <= 0)
  {
    trackSize = EditContentComponent::defaultTrackSize;
    settings.setProperty(
        id::trackSize,
        trackSize,
        nullptr);
  }

  auto newBounds = this->getBounds();
  newBounds.setWidth(trackSize * this->tracks.size());
  this->setBounds(newBounds);
}

BLOOPER_NAMESPACE_END
