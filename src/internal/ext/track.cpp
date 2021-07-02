#include <blooper/internal/abstract/time.hpp>
#include <blooper/internal/ext/track.hpp>
#include <blooper/internal/ext/clip.hpp>

BLOOPER_EXT_NAMESPACE_BEGIN

// Attributes

bool ToggleTrackArmedAction::perform()
{
  if (this->trackRef.wasObjectDeleted()) return false;

  auto&      track = *this->trackRef;
  const auto shouldBeArmed = !this->wasArmed;

  for (auto instance : track.edit.getAllInputDevices())
    if (instance->isOnTargetTrack(track))
      instance->setRecordingEnabled(track, shouldBeArmed);

  track.state.setProperty(
      id::armed,
      shouldBeArmed,
      nullptr);

  return true;
}


// Clips

inline te::Clip* leaveOnlyFirstClip(te::ClipTrack& track)
{
  if (!hasClips(track)) return nullptr;

  te::Clip* candidate = nullptr;
  auto      clips = track.getClips();

  for (const auto clip : clips)
  {
    if (isOnLoopStart(*clip))
    {
      candidate = clip;
      break;
    }
  }

  if (candidate == nullptr)
  {
    candidate = clips.getFirst();
    moveToLoopStart(*candidate);
  }

  for (const auto clip : clips)
    if (clip != candidate)
      clip->removeFromParentTrack();

  return candidate;
}

inline te::Clip* mergeFirstAndLastClip(te::ClipTrack& track)
{
  if (!hasClips(track)) return nullptr;

  te::Clip* candidatePtr;

  if (track.getClips().size() == 1)
  {
    const auto firstPtr = track.getClips().getFirst();
    auto&      first = *firstPtr;
    moveToLoopStart(
        first);

    candidatePtr = firstPtr;
  }
  else if (track.getClips().size() == 2)
  {
    const auto lastPtr = track.getClips().getLast();
    auto&      last = *lastPtr;

    const auto firstPtr = track.getClips().getFirst();
    auto&      first = *firstPtr;

    moveToLoopStart(
        first,
        getLength(first) *
            getLength(last));

    candidatePtr = firstPtr;
  }
  else
  {
    candidatePtr = leaveOnlyFirstClip(track);
  }

  return candidatePtr;
}

inline te::Clip* fixCandidateClip(te::ClipTrack& track)
{
  if (!hasClips(track)) return nullptr;

  te::Clip*  candidate = nullptr;
  const auto mode = getTrackMode(track);

  switch (mode)
  {
    case TrackMode::sync:
      candidate = leaveOnlyFirstClip(track);
      break;

    case TrackMode::oneShot:
      candidate = mergeFirstAndLastClip(track);
      break;

    case TrackMode::free:
      candidate = leaveOnlyFirstClip(track);
      break;

    default:
      break;
  }

  return candidate;
}

inline void repeatCandidateClip(
    te::ClipTrack& track,
    te::Clip&      candidate,
    double         beats)
{
  beats = std::max(beats, loopEndBeat);
  auto& tempo = track.edit.tempoSequence;
  moveToLoopStart(candidate, beats);
  auto lastEnd = beats;

  for (auto newStart = beats;
       newStart + beats < loopEndBeat;
       newStart += beats) // NOLINT(cert-flp30-c)
    setPositionBeats(
        copy(candidate),
        newStart,
        newStart + beats);

  setPositionBeats(
      copy(candidate),
      lastEnd,
      16);
}

inline void positionCandidateClip(
    te::ClipTrack& track,
    te::Clip&      clip,
    double         startBeat)
{
  const auto endBeat = startBeat + clip.getLengthInBeats();
  setPositionBeats(clip, startBeat, endBeat);

  if (endBeat > loopEndBeat)
  {
    moveToLoopStart(
        *track.splitClip(
            clip,
            track.edit.tempoSequence.beatsToTime(
                loopEndBeat)));
  }
}


// Interval and Mode

Interval setTrackInterval(
    te::Track& track,
    Interval   interval)
{
  using Converter = juce::VariantConverter<Interval>;

  const auto mode = getTrackMode(track);
  if (mode == TrackMode::sync)
  {
    if (auto clipTrack = isClipTrack(track))
      if (auto candidate = fixCandidateClip(*clipTrack))
        repeatCandidateClip(
            *clipTrack,
            *candidate,
            getBeats(interval));

    if (auto subTracks = track.getSubTrackList())
      subTracks->visitAllTopLevel(
          [interval](te::Track& subTrack) {
            setTrackInterval(subTrack, interval);
            return true; // continue
          });
  }

  track.state.setProperty(
      id::mode,
      Converter::toVar(interval),
      nullptr);
}


TrackMode setTrackMode(
    te::Track& track,
    TrackMode  mode)
{
  using Converter = juce::VariantConverter<TrackMode>;

  if (auto clipTrack = isClipTrack(track))
    if (auto candidate = fixCandidateClip(*clipTrack))
      switch (mode)
      {
        case TrackMode::sync:
          repeatCandidateClip(
              *clipTrack,
              *candidate,
              getBeats(getTrackInterval(track)));
          break;

        case TrackMode::free:
          repeatCandidateClip(
              *clipTrack,
              *candidate,
              candidate->getLengthInBeats());
          break;

        default:
          break;
      }

  if (auto subTracks = track.getSubTrackList())
    subTracks->visitAllTopLevel(
        [mode](te::Track& subTrack) {
          setTrackMode(subTrack, mode);
          return true; // continue
        });

  track.state.setProperty(
      id::mode,
      Converter::toVar(mode),
      nullptr);
}


// "Transport"


void preparePlay(te::Track& track)
{
  if (getTrackMode(track) == TrackMode::oneShot)
    if (auto clipTrack = isClipTrack(track))
      if (auto candidate = fixCandidateClip(*clipTrack))
        positionCandidateClip(
            *clipTrack,
            *candidate,
            track.edit.getTransport().getCurrentPosition());
}

inline juce::Array<te::Track*> unpackTakesIntoSubTracks(
    te::Clip&  clip,
    te::Track& track)
{
  juce::Array<te::Track*> createdTracks{};

  if (auto waveClip = isWaveClip(clip))
  {
    auto stateCopy = clip.state.createCopy();
    if (!stateCopy.isValid()) return {};

    auto&      edit = track.edit;
    auto&      compManager = waveClip->getCompManager();
    const auto takes = waveClip->getTakes();
    const auto trackPtr = std::addressof(track);

    stateCopy.removeChild(
        stateCopy.getChildWithName(te::IDs::TAKES),
        nullptr);

    for (int i = 0; i < takes.size(); ++i)
    {
      if (compManager.isTakeComp(i))
        continue;

      const auto targetTrack =
          edit.insertNewAudioTrack(
              {trackPtr, nullptr},
              nullptr);
      createdTracks.add(targetTrack);

      targetTrack->insertWaveClip(
          waveClip->getTakeDescriptions()[i],
          takes[i],
          {{0.0, getLength(*waveClip)}},
          false);
    }
  }
  else if ([[maybe_unused]] auto midiClip = isMidiClip(clip))
  {
    // TODO
    return {};
  }

  clip.removeFromParentTrack();
  return createdTracks;
}

void unpackRecordedTakes(te::Track& track)
{
  const auto mode = getTrackMode(track);
  const auto interval = getTrackInterval(track);
  const auto intervalBeats = getBeats(interval);

  if (auto clipTrack = isClipTrack(track))
  {
    if (mode == TrackMode::sync)
    {
      for (auto currentSplit = 0.0;
           currentSplit < loopEndBeat;
           currentSplit += intervalBeats) // NOLINT(cert-flp30-c)
        splitBeat(*clipTrack, currentSplit);
    }

    juce::Array<te::Track*> createdSubTracks{};
    const auto              clipsToUnpack = clipTrack->getClips();
    for (const auto clip : clipsToUnpack)
      createdSubTracks.addArray(
          unpackTakesIntoSubTracks(
              *clip,
              track));

    for (auto subTrack : createdSubTracks)
    {
      if (auto subClipTrack = isClipTrack(subTrack))
      {
        if (auto candidate = fixCandidateClip(*subClipTrack))
        {
          switch (mode)
          {
            case TrackMode::sync:
              {
                repeatCandidateClip(
                    *subClipTrack,
                    *candidate,
                    intervalBeats);
              }
              break;

            case TrackMode::free:
              {
                const auto candidateLengthBeats =
                    candidate->getLengthInBeats();

                repeatCandidateClip(
                    *subClipTrack,
                    *candidate,
                    candidateLengthBeats);
              }
              break;

            default:
              break;
          }
        }
      }
    }
  }
}

BLOOPER_EXT_NAMESPACE_END
