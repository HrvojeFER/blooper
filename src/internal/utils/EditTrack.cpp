#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

EditTrack::EditTrack(
    AbstractContext&   context,
    State              state,
    JuceProjectItemRef item)
    : ContextualBase(context),
      StatefulBase(move(state)),
      item(move(item))
{
  // BLOOPER_ASSERT(this->item->isEdit()); - wtf

  auto& engine = this->getContext().getEngine();
  auto  undoManager = std::addressof(this->getContext().getUndoManager());


  this->edit =
      JuceEditRef{
          te::loadEditFromFile(
              engine,
              this->item->getSourceFile())
              .release()};
  BLOOPER_ASSERT(edit);

  this->transport = std::addressof(edit->getTransport());
  BLOOPER_ASSERT(transport);

  this->transport->ensureContextAllocated();
  this->edit->restartPlayback();

  this->edit->ensureNumberOfAudioTracks(1);
  this->edit->getTrackList().visitAllTopLevel(
      [this](JuceTrack& current) {
        if (auto _audio = dynamic_cast<JuceAudioTrack*>(
                std::addressof(current)))
        {
          this->audio = _audio;
          return false;
        }
        return true;
      });
  BLOOPER_ASSERT(audio);


  this->id.referTo(
      this->getState(),
      te::IDs::uid,
      undoManager);


  this->mode.referTo(
      this->getState(),
      id::mode,
      undoManager,
      TrackMode::sync);

  this->interval.referTo(
      this->getState(),
      id::interval,
      undoManager,
      Interval::fourBeats);

  this->playback.referTo(
      this->getState(),
      id::playback,
      undoManager,
      TrackPlayback::paused);


  this->muted.referTo(
      this->getState(),
      id::muted,
      undoManager,
      false);
  this->audio->setMute(this->muted);

  this->armed.referTo(
      this->getState(),
      id::armed,
      undoManager,
      true);
  for (auto device : this->edit->getAllInputDevices())
    if (device->isOnTargetTrack(*this->audio))
      device->setRecordingEnabled(*this->audio, this->armed);


  this->syncToken = invalidToken;
  this->recordToken = invalidToken;
  this->synchronize();
}

EditTrack::~EditTrack()
{
  te::EditFileOperations(*this->edit)
      .save(
          false,
          true,
          false);

  this->notifyListenersOfDeletion();
}


[[maybe_unused, nodiscard]] bool EditTrack::isSoloed() const noexcept
{
  auto& manager = this->getContext().getEditManager();

  return manager.soloed == this->id;
}

[[maybe_unused]] void EditTrack::setSoloed(bool toSolo) noexcept
{
  auto& manager = this->getContext().getEditManager();

  if (!toSolo && this->isSoloed())
    manager.soloed = EditTrack::invalidId;
  else
    manager.soloed = this->getId();
}

[[maybe_unused]] void EditTrack::toggleSoloed() noexcept
{
  this->setSoloed(!this->isSoloed());
}


void EditTrack::synchronize()
{
  // Clean up previous sync

  auto& sync = this->getContext().getSynchronizer();
  if (isValid(this->syncToken)) sync.cancel(this->syncToken);


  switch (this->mode)
  {
      // Sync - Sync with other edits

    case TrackMode::sync:
      {
        sync.every(
            this->interval,
            [weak = juce::WeakReference<EditTrack>(this)] {
              if (weak.wasObjectDeleted()) return;
              weak->transport->setCurrentPosition(0);
            });
      }
      break;

      // Free - Loop over longest clip

    case TrackMode::free:
      {
        te::Clip* shortestClip = nullptr;
        for (auto clip : this->audio->getClips())
          if (shortestClip == nullptr ||
              clip->getMaximumLength() < shortestClip->getMaximumLength())
            shortestClip = clip;

        if (shortestClip)
        {
          this->transport->setLoopRange(
              {0,
               shortestClip->getMaximumLength()});
        }
      }
      break;

      // One Shot - no sync needed

    default:
      break;
  }
}


// ValueTreeListener

void EditTrack::valueTreePropertyChanged(
    juce::ValueTree&        tree,
    const juce::Identifier& _id)
{
  auto& manager = this->getContext().getEditManager();
  auto& sync = this->getContext().getSynchronizer();

  if (tree == this->getState())
  {
    if (_id == id::mode)
    {
      this->synchronize();
    }
    else if (_id == id::interval)
    {
      if (this->mode == TrackMode::sync)
        this->synchronize();
    }
    else if (_id == id::playback)
    {
      if (this->playback == TrackPlayback::playing)
      {
        this->transport->play(false);
      }
      if (this->playback == TrackPlayback::paused)
      {
        this->transport->stop(
            false,
            false,
            true);
      }
      if (this->playback == TrackPlayback::recording)
      {
        if (this->mode == TrackMode::sync)
        {
          this->recordToken = sync.on(
              static_cast<Delay>(this->interval.get()),
              [weak = juce::WeakReference<EditTrack>(this)] {
                if (weak.wasObjectDeleted()) return;
                weak->transport->record(false);

                weak->recordToken = weak->getContext().getSynchronizer().on(
                    static_cast<Delay>(weak->interval.get()),
                    [weak] {
                      if (weak.wasObjectDeleted()) return;
                      weak->transport->play(false);
                    });
              });
        }
        else
        {
          this->transport->record(false);
        }
      }

      else if (_id == id::muted)
      {
        if (!manager.isAnyTrackSoloed())
          this->audio->setMute(!this->muted);
      }
      else if (_id == id::armed)
      {
        for (auto device : this->edit->getAllInputDevices())
          if (device->isOnTargetTrack(*this->audio))
            device->setRecordingEnabled(*this->audio, this->armed.get());
      }
    }
  }
}


// Selectable

juce::String EditTrack::getSelectableDescription()
{
  // TODO?
  return juce::String("Edit Track");
}

BLOOPER_NAMESPACE_END
