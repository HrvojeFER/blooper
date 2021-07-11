//#include <blooper/internal/utils/EditTrack.hpp>
//
//#include <blooper/internal/abstract/id.hpp>
//
//#include <blooper/context/behaviour/EditManager.hpp>
//#include <blooper/context/behaviour/Synchronizer.hpp>
//
//BLOOPER_NAMESPACE_BEGIN
//
//EditTrack::EditTrack(
//    AbstractContext&   context,
//    State              state,
//    JuceProjectItemRef item)
//    : ContextualBase(context),
//      StatefulBase(move(state)),
//      item(move(item))
//{
//  // the analyzer thinks this is never true for some reason...
//  // BLOOPER_ASSERT(this->item->isEdit());
//
//  auto& engine = this->getContext().getEngine();
//  auto  undoManager = this->getContext().getUndoManagerPtr();
//
//
//  this->edit =
//      JuceEditRef{
//          te::loadEditFromFile(
//              engine,
//              this->item->getSourceFile())
//              .release()};
//  BLOOPER_ASSERT(this->edit);
//  // this is the easiest way to do this
//  this->edit->setProjectItemID(this->item->getID());
//
//  this->transport = std::addressof(edit->getTransport());
//  BLOOPER_ASSERT(transport);
//
//  this->transport->ensureContextAllocated();
//  this->edit->restartPlayback();
//
//  this->edit->ensureNumberOfAudioTracks(1);
//  this->edit->getTrackList().visitAllTopLevel(
//      [this](JuceTrack& current) {
//        if (auto _audio = dynamic_cast<JuceAudioTrack*>(
//                std::addressof(current)))
//        {
//          this->audio = _audio;
//          return false;
//        }
//        return true;
//      });
//  BLOOPER_ASSERT(audio);
//
//
//  this->id.referTo(
//      this->getState(),
//      te::IDs::uid,
//      nullptr);
//
//
//  this->muted.referTo(
//      this->getState(),
//      id::muted,
//      undoManager,
//      false);
//
//  this->armed.referTo(
//      this->getState(),
//      id::armed,
//      undoManager,
//      false);
//
//
//  this->mode.referTo(
//      this->getState(),
//      id::mode,
//      undoManager,
//      TrackMode::sync);
//
//  this->interval.referTo(
//      this->getState(),
//      id::interval,
//      undoManager,
//      Interval::fourBeats);
//
//  this->playback.referTo(
//      this->getState(),
//      id::playback,
//      undoManager,
//      TrackPlayback::paused);
//  this->playback = TrackPlayback::paused;
//
//
//  this->syncToken = invalidToken;
//  this->playbackToken = invalidToken;
//  this->synchronize();
//}
//
//EditTrack::~EditTrack()
//{
//  this->playback = TrackPlayback::paused;
//
//  auto& sync = this->getContext().getSynchronizer();
//  sync.cancel(this->syncToken);
//  sync.cancel(this->playbackToken);
//
//  te::EditFileOperations(*this->edit)
//      .save(
//          false,
//          true,
//          false);
//
//  this->notifyListenersOfDeletion();
//}
//
//
//[[maybe_unused, nodiscard]] bool EditTrack::isSoloed() const
//{
//  auto& manager = this->getContext().getEditManager();
//
//  return manager.soloed == this->id;
//}
//
//[[maybe_unused]] void EditTrack::setSoloed(bool toSolo)
//{
//  auto& manager = this->getContext().getEditManager();
//
//  if (!toSolo && this->isSoloed())
//    manager.soloed = EditTrack::invalidId;
//  else
//    manager.soloed = this->getId();
//}
//
//[[maybe_unused]] void EditTrack::toggleSoloed()
//{
//  this->setSoloed(!this->isSoloed());
//}
//
//
//[[maybe_unused]] bool EditTrack::isClear()
//{
//  return this->getAudio().getClips().size() == 0;
//}
//
//[[maybe_unused]] void EditTrack::clear()
//{
//  this->getAudio().deleteRegion(
//      this->getAudio().getTotalRange(),
//      nullptr);
//
//  this->playback = TrackPlayback::paused;
//}
//
//
//[[maybe_unused, nodiscard]] double
//EditTrack::getProgress() const noexcept
//{
//  if (this->transport->looping)
//  {
//    return this->transport->getCurrentPosition() /
//           this->transport->getLoopRange().getLength();
//  }
//  else if (this->mode == TrackMode::sync)
//  {
//    return this->getContext()
//        .getSynchronizer()
//        .getProgress(this->interval);
//  }
//  else if (auto shortestClip = this->getShortestClip();
//           this->transport->isPlaying())
//  {
//    return this->transport->getCurrentPosition() /
//           shortestClip->getMaximumLength();
//  }
//
//  return 0.0;
//}
//
//
//te::Clip* EditTrack::getShortestClip() const noexcept
//{
//  te::Clip* shortestClip = nullptr;
//
//  for (auto clip : this->audio->getClips())
//    if (shortestClip == nullptr ||
//        clip->getMaximumLength() < shortestClip->getMaximumLength())
//      shortestClip = clip;
//
//  return shortestClip;
//}
//
//void EditTrack::synchronize()
//{
//  // Clean up previous sync
//
//  auto& sync = this->getContext().getSynchronizer();
//  sync.cancelAsync(this->syncToken);
//  this->syncToken = invalidToken;
//
//
//  switch (this->mode)
//  {
//    case TrackMode::sync:
//      {
//        this->transport->looping = true;
//        this->transport->setLoopRange(
//            {0,
//             getBeats(this->interval) / 2.0});
//      }
//      break;
//
//    case TrackMode::free:
//      {
//        this->transport->looping = true;
//        if (auto shortestClip = this->getShortestClip())
//        {
//          this->transport->setLoopRange(
//              {0,
//               shortestClip->getMaximumLength()});
//        }
//        else
//        {
//          this->transport->setLoopRange(
//              {0,
//               sync.getMilliseconds(this->interval)});
//        }
//      }
//      break;
//
//    case TrackMode::oneShot:
//      {
//        this->transport->looping = false;
//      }
//      break;
//
//    default:
//      break;
//  }
//}
//
//
//// ValueTreeListener
//
//void EditTrack::valueTreePropertyChanged(
//    juce::ValueTree&        tree,
//    const juce::Identifier& _id)
//{
//  auto& manager = this->getContext().getEditManager();
//  auto& sync = this->getContext().getSynchronizer();
//
//  if (tree == this->getState())
//  {
//    if (_id == id::mode)
//    {
//      this->synchronize();
//    }
//    else if (_id == id::interval)
//    {
//      if (this->mode == TrackMode::sync)
//      {
//        this->synchronize();
//      }
//    }
//
//    else if (_id == id::playback)
//    {
//      if (this->playback == TrackPlayback::paused)
//      {
//        this->transport->stop(
//            false,
//            false,
//            true);
//      }
//
//      else if (this->playback == TrackPlayback::scheduledPlaying)
//      {
//        if (this->mode == TrackMode::sync)
//        {
//          this->playbackToken =
//              sync.onAsync(
//                  static_cast<Delay>(this->interval.get()),
//                  [weak = juce::WeakReference<EditTrack>(this)] {
//                    if (weak.wasObjectDeleted())
//                      return;
//
//                    weak->playbackToken = invalidToken;
//                    if (weak->playback != TrackPlayback::scheduledPlaying)
//                      return;
//
//                    weak->playback = TrackPlayback::playing;
//                  });
//        }
//        else
//        {
//          this->playback = TrackPlayback::playing;
//        }
//      }
//      else if (this->playback == TrackPlayback::playing)
//      {
//        this->transport->play(false);
//      }
//
//      else if (this->playback == TrackPlayback::scheduledRecording)
//      {
//        if (this->mode == TrackMode::sync)
//        {
//          this->playbackToken =
//              sync.onAsync(
//                  static_cast<Delay>(this->interval.get()),
//                  [weak = juce::WeakReference<EditTrack>(this)] {
//                    if (weak.wasObjectDeleted())
//                      return;
//
//                    weak->playbackToken = invalidToken;
//
//                    if (weak->playback != TrackPlayback::scheduledRecording)
//                      return;
//
//                    weak->playback = TrackPlayback::recording;
//                  });
//        }
//        else
//        {
//          this->playback = TrackPlayback::recording;
//        }
//      }
//      else if (this->playback == TrackPlayback::recording)
//      {
//        this->transport->record(false);
//
//        if (this->mode == TrackMode::sync)
//        {
//          this->playbackToken =
//              sync.onAsync(
//                  static_cast<Delay>(this->interval.get()),
//                  [weak = juce::WeakReference<EditTrack>(this)] {
//                    if (weak.wasObjectDeleted())
//                      return;
//
//                    weak->playbackToken = invalidToken;
//
//                    if (weak->playback != TrackPlayback::recording)
//                      return;
//
//                    weak->playback = TrackPlayback::paused;
//                  });
//        }
//      }
//    }
//
//    else if (_id == id::muted)
//    {
//      if (!manager.isAnyTrackSoloed())
//        this->audio->setMute(this->muted);
//    }
//    else if (_id == id::armed)
//    {
//      for (auto device : this->edit->getAllInputDevices())
//        if (device->isOnTargetTrack(*this->audio))
//          device->setRecordingEnabled(*this->audio, this->armed);
//    }
//  }
//}
//
//
//// Selectable
//
//juce::String EditTrack::getSelectableDescription()
//{
//  return juce::String(this->getAudio().getName());
//}
//
//BLOOPER_NAMESPACE_END
