#ifndef BLOOPER_EXT_TRACK_HPP
#define BLOOPER_EXT_TRACK_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/meta.hpp>
#include <blooper/internal/abstract/predicates.hpp>
#include <blooper/internal/abstract/visitors.hpp>
#include <blooper/internal/abstract/id.hpp>
#include <blooper/internal/abstract/time.hpp>
#include <blooper/internal/ext/clip.hpp>
#include <blooper/internal/ext/plugin.hpp>


BLOOPER_EXT_NAMESPACE_BEGIN

// TODO: make actions properly undoable


// Types

[[maybe_unused, nodiscard]] inline const te::ClipTrack*
isClipTrack(const te::Track* track)
{
  return dynamic_cast<const te::ClipTrack*>(track);
}

[[maybe_unused, nodiscard]] inline te::ClipTrack*
isClipTrack(te::Track* track)
{
  return dynamic_cast<te::ClipTrack*>(track);
}

[[maybe_unused, nodiscard]] inline const te::ClipTrack*
isClipTrack(const te::Track& track)
{
  return dynamic_cast<const te::ClipTrack*>(std::addressof(track));
}

[[maybe_unused, nodiscard]] inline te::ClipTrack*
isClipTrack(te::Track& track)
{
  return dynamic_cast<te::ClipTrack*>(std::addressof(track));
}


[[maybe_unused, nodiscard]] inline const te::AudioTrack*
isAudioTrack(const te::Track* track)
{
  return dynamic_cast<const te::AudioTrack*>(track);
}

[[maybe_unused, nodiscard]] inline te::AudioTrack*
isAudioTrack(te::Track* track)
{
  return dynamic_cast<te::AudioTrack*>(track);
}

[[maybe_unused, nodiscard]] inline const te::AudioTrack*
isAudioTrack(const te::Track& track)
{
  return dynamic_cast<const te::AudioTrack*>(std::addressof(track));
}

[[maybe_unused, nodiscard]] inline te::AudioTrack*
isAudioTrack(te::Track& track)
{
  return dynamic_cast<te::AudioTrack*>(std::addressof(track));
}


// Clearing content

[[maybe_unused, nodiscard]] inline bool hasClips(const te::ClipTrack& track)
{
  return track.getClips().isEmpty();
}

[[maybe_unused, nodiscard]] inline bool hasClips(const te::Track& track)
{
  if (auto clipTrack = isClipTrack(track))
    return hasClips(*clipTrack);

  return true;
}

// TODO: check for clips in clip tracks?
[[maybe_unused, nodiscard]] inline bool hasSubTracks(const te::Track& track)
{
  return track.hasSubTracks();
}

[[maybe_unused, nodiscard]] inline bool isClear(const te::Track& track)
{
  if (track.hasSubTracks())
    return false;

  if (hasClips(track))
    return false;

  return true;
}

[[maybe_unused]] inline bool clearSubTracks(te::Track& track)
{
  const auto undo = std::addressof(track.edit.getUndoManager());

  if (track.hasSubTracks())
  {
    auto subTracks = track.getAllSubTracks(false);
    for (const auto subTrack : subTracks)
      track.state.removeChild(subTrack->state, undo);

    return true;
  }

  return false;
}

[[maybe_unused]] inline bool clearClips(te::ClipTrack& track)
{
  if (!hasClips(track))
    return false;

  auto clips = track.getClips();
  for (const auto clip : clips)
    clip->removeFromParentTrack();

  return true;
}

[[maybe_unused]] inline bool clearClips(te::Track& track)
{
  if (const auto clipTrack = isClipTrack(track))
    return clearClips(*clipTrack);

  return false;
}

[[maybe_unused]] inline bool clear(te::Track& track)
{
  if (clearClips(track))
    return true;
  if (clearSubTracks(track))
    return true;

  return false;
}


[[maybe_unused]] inline void split(
    te::ClipTrack& track,
    double         time)
{
  track.splitAt(time);
}

[[maybe_unused]] inline void splitBeat(
    te::ClipTrack& track,
    double         beat)
{
  auto& tempo = track.edit.tempoSequence;

  track.splitAt(tempo.beatsToTime(beat));
}


// Plugins

[[maybe_unused]] inline void
addPlugin(
    te::Track&            track,
    te::Plugin::Ptr       plugin,
    te::SelectionManager* selection)
{
  track.pluginList
      .insertPlugin(
          plugin,
          -1,
          selection);
}


// Attributes

[[maybe_unused, nodiscard]] inline bool isArmed(const te::Track& track)
{
  return track.state.getProperty(
      id::armed,
      false);
}

class ToggleTrackArmedAction final : public juce::UndoableAction
{
 public:
  inline explicit ToggleTrackArmedAction(te::Track& track)
      : trackRef(std::addressof(track)),
        wasArmed(isArmed(track))
  {
  }

  ~ToggleTrackArmedAction() final = default;


  bool perform() final;

  inline bool undo() final
  {
    toggle(this->wasArmed);
    return this->perform();
  }

  inline int getSizeInUnits() final
  {
    return sizeof(ToggleTrackArmedAction);
  }


 private:
  [[maybe_unused]] juce::WeakReference<te::Track> trackRef;

  [[maybe_unused]] bool wasArmed;
};

[[maybe_unused]] inline void toggleArmed(te::Track& track)
{
  track.edit.getUndoManager().perform(
      new ToggleTrackArmedAction(track));
}


[[maybe_unused, nodiscard]] inline bool isMuted(const te::Track& track)
{
  return track.isMuted(false);
}

[[maybe_unused]] inline void toggleMuted(te::Track& track)
{
  track.setMute(!isMuted(track));
}


[[maybe_unused, nodiscard]] inline bool isSoloed(const te::Track& track)
{
  return track.isSolo(false);
}

[[maybe_unused]] inline void toggleSoloed(te::Track& track)
{
  track.setSolo(!isSoloed(track));
}


// Mode and Interval

[[maybe_unused, nodiscard]] inline Interval getTrackInterval(te::Track& track)
{
  using Converter = juce::VariantConverter<Interval>;

  return Converter::fromVar(
      track.state.getProperty(
          id::interval,
          Converter::toVar(
              Interval::fourBeats)));
}

[[maybe_unused, nodiscard]] inline TrackMode getTrackMode(te::Track& track)
{
  using Converter = juce::VariantConverter<TrackMode>;

  return Converter::fromVar(
      track.state.getProperty(
          id::mode,
          Converter::toVar(
              TrackMode::sync)));
}


[[maybe_unused]] Interval setTrackInterval(
    te::Track& track,
    Interval   interval);

[[maybe_unused]] inline Interval cycleTrackInterval(te::Track& track)
{
  const auto currentTrackInterval{
      static_cast<int>(getTrackInterval(track))};

  auto nextTrackInterval{
      currentTrackInterval == 64 ?
          0 :
          currentTrackInterval * 2};

  return setTrackInterval(
      track,
      static_cast<Interval>(nextTrackInterval));
}


[[maybe_unused]] TrackMode setTrackMode(
    te::Track& track,
    TrackMode  mode);

[[maybe_unused]] inline TrackMode cycleTrackMode(te::Track& track)
{
  const auto currentTrackMode{getTrackMode(track)};
  auto       nextTrackMode{TrackMode::sync};

  switch (currentTrackMode)
  {
    case TrackMode::sync:
      nextTrackMode = TrackMode::oneShot;
      break;

    case TrackMode::oneShot:
      nextTrackMode = TrackMode::free;
      break;

    case TrackMode::free:
      nextTrackMode = TrackMode::sync;
      break;

    default:
      break;
  }

  return setTrackMode(track, nextTrackMode);
}


// Lifetime

[[maybe_unused]] inline void
init(te::AudioTrack& audioTrack)
{
  for (auto inputInstance : audioTrack.edit.getAllInputDevices())
  {
    if (inputInstance->getInputDevice().getDeviceType() ==
        te::InputDevice::waveDevice)
    {
      inputInstance->setTargetTrack(
          audioTrack,
          // idk what this index thing is
          0,
          false);

      inputInstance->setRecordingEnabled(
          audioTrack,
          isArmed(audioTrack));
    }
  }
}


// Transport

[[maybe_unused]] void prepareForPlaying(te::Track& track);

[[maybe_unused]] void unpackRecordedTakes(te::Track& track);

[[maybe_unused]] double getProgress(te::Track& track);


// Visit

template<VisitDepth Depth = defaultVisitDepth, typename TVisitor>
[[maybe_unused]] inline bool visit(te::Track& track, TVisitor visitor)
{
  static_assert(
      BLOOPER_TYPEID(visitor) ^
          isVisitorOf ^
          meta::type_c<te::Track&>,
      "te::Track visit requires a Visitor of te::Track&");


  if (const auto subTracks = track.getSubTrackList())
  {
    for (const auto subTrack : *subTracks)
    {
      if (subTrack)
      {
        if (!callVisitor(visitor, *subTrack))
          return stopVisit;

        if constexpr (Depth == VisitDepth::deep)
          if (!visit<Depth>(*subTrack, visitor))
            return stopVisit;
      }
    }
  }

  return continueVisit;
}

template<VisitDepth Depth = defaultVisitDepth, typename TPredicate>
[[maybe_unused]] inline te::Track* find(te::Track& track, TPredicate predicate)
{
  static_assert(
      BLOOPER_TYPEID(predicate) ^
          isPredicateOf ^
          meta::type_c<te::Track&>,
      "te::Track find requires a Predicate of te::Track&");


  te::Track* result;

  visit<Depth>(
      track,
      [&result, predicate = move(predicate)](
          te::Track& track) {
        if (predicate(track))
        {
          result = std::addressof(track);
          return stopVisit;
        }

        return continueVisit;
      });

  return result;
}

template<typename TVisitor>
[[maybe_unused]] inline bool visitAncestors(te::Track& track, TVisitor visitor)
{
  static_assert(
      BLOOPER_TYPEID(visitor) ^
          isVisitorOf ^
          meta::type_c<te::Track&>,
      "te::Track visitAncestors requires a Visitor of te::Track&");


  if (const auto parentTrack = track.getParentTrack())
  {
    if (!callVisitor(visitor, *parentTrack)) return stopVisit;

    visitAncestors(track, move(visitor));
  }

  return continueVisit;
}

template<typename TPredicate>
[[maybe_unused]] inline te::Track* findAncestor(
    te::Track& track,
    TPredicate predicate)
{
  static_assert(
      BLOOPER_TYPEID(predicate) ^
          isPredicateOf ^
          meta::type_c<te::Track&>,
      "te::Track findAncestor requires a Predicate of te::Track&");


  te::Track* result;

  visitAncestors(
      track,
      [&result, predicate = move(predicate)](
          te::Track& track) {
        if (predicate(track))
        {
          result = std::addressof(track);
          return stopVisit;
        }

        return continueVisit;
      });

  return result;
}

BLOOPER_EXT_NAMESPACE_END

#endif // BLOOPER_EXT_TRACK_HPP
