#include <blooper/components/tracks/TrackClipsDetailContentComponent.hpp>

#include <blooper/internal/abstract/id.hpp>

BLOOPER_NAMESPACE_BEGIN

TrackClipsDetailContentComponent::TrackClipsDetailContentComponent(
    AbstractContext&                        context,
    State                                   state,
    JuceTrackRef                            track,
    TrackClipsDetailContentComponentOptions options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options)),

      track(move(track)),

      clipsUpdate(false),
      subTracksUpdate(false)
{
  this->appearanceSettings =
      this->getContext()
          .getSettings()
          .getOrCreateChildWithName(
              id::appearance,
              nullptr);


  this->clipSize.referTo(
      this->appearanceSettings,
      id::clipSize,
      nullptr,
      20);


  this->updateClips();
  this->updateSubTracks();


  this->track->state.addListener(this);

  this->appearanceSettings.addListener(this);
}

TrackClipsDetailContentComponent::~TrackClipsDetailContentComponent()
{
  this->appearanceSettings.removeListener(this);

  this->track->state.removeListener(this);
}


void TrackClipsDetailContentComponent::updateClips()
{
}

void TrackClipsDetailContentComponent::updateSubTracks()
{
}


// Component

void TrackClipsDetailContentComponent::resized()
{
}


// ValueTreeListener

void TrackClipsDetailContentComponent::valueTreePropertyChanged(
    juce::ValueTree&        tree,
    const juce::Identifier& id)
{
  if (tree == this->track->state)
  {
    if (id == te::IDs::TRACK)
    {
      this->markAndUpdate(this->subTracksUpdate);
    }
    else if (id == te::IDs::AUDIOCLIP)
    {
      this->markAndUpdate(this->clipsUpdate);
    }
  }
  if (tree == this->appearanceSettings)
  {
    if (id == id::clipSize)
    {
      this->markAndUpdate(this->clipsUpdate);
    }
  }
}

void TrackClipsDetailContentComponent::valueTreeChildAdded(
    juce::ValueTree& tree,
    juce::ValueTree& child)
{
  if (tree == this->track->state)
  {
    if (child.hasType(te::IDs::TRACK))
    {
      this->markAndUpdate(this->subTracksUpdate);
    }
    else if (child.hasType(te::IDs::AUDIOCLIP))
    {
      this->markAndUpdate(this->clipsUpdate);
    }
  }
}

void TrackClipsDetailContentComponent::valueTreeChildRemoved(
    juce::ValueTree& tree,
    juce::ValueTree& child,
    int              index)
{
  if (tree == this->track->state)
  {
    if (child.hasType(te::IDs::TRACK))
    {
      this->markAndUpdate(this->subTracksUpdate);
    }
    else if (child.hasType(te::IDs::AUDIOCLIP))
    {
      this->markAndUpdate(this->clipsUpdate);
    }
  }
}

void TrackClipsDetailContentComponent::valueTreeChildOrderChanged(
    juce::ValueTree& tree,
    int              childAIndex,
    int              childBIndex)
{
  if (tree == this->track->state)
  {
    auto childA = tree.getChild(childAIndex);
    auto childB = tree.getChild(childBIndex);

    if (childA.hasType(te::IDs::TRACK) ||
        childB.hasType(te::IDs::TRACK))
    {
      this->markAndUpdate(this->subTracksUpdate);
    }
    else if (
        childA.hasType(te::IDs::AUDIOCLIP) ||
        childB.hasType(te::IDs::AUDIOCLIP))
    {
      this->markAndUpdate(this->clipsUpdate);
    }
  }
}


// FlaggedAsyncUpdater

void TrackClipsDetailContentComponent::handleAsyncUpdate()
{
  if (util::FlaggedAsyncUpdater::compareAndReset(this->clipsUpdate))
  {
    this->updateClips();
  }
  if (util::FlaggedAsyncUpdater::compareAndReset(this->subTracksUpdate))
  {
    this->updateSubTracks();
  }
}

BLOOPER_NAMESPACE_END
