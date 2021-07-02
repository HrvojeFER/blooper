#ifndef BLOOPER_EXT_CLIP_HPP
#define BLOOPER_EXT_CLIP_HPP

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/const.hpp>

BLOOPER_EXT_NAMESPACE_BEGIN

// Types

[[maybe_unused]] inline te::WaveAudioClip*
isWaveClip(te::Clip* clip)
{
  return dynamic_cast<te::WaveAudioClip*>(clip);
}

[[maybe_unused]] inline const te::WaveAudioClip*
isWaveClip(const te::Clip* clip)
{
  return dynamic_cast<const te::WaveAudioClip*>(clip);
}

[[maybe_unused]] inline te::WaveAudioClip*
isWaveClip(te::Clip& clip)
{
  return dynamic_cast<te::WaveAudioClip*>(std::addressof(clip));
}

[[maybe_unused]] inline const te::WaveAudioClip*
isWaveClip(const te::Clip& clip)
{
  return dynamic_cast<const te::WaveAudioClip*>(std::addressof(clip));
}


[[maybe_unused]] inline te::MidiClip*
isMidiClip(te::Clip* clip)
{
  return dynamic_cast<te::MidiClip*>(clip);
}

[[maybe_unused]] inline const te::MidiClip*
isMidiClip(const te::Clip* clip)
{
  return dynamic_cast<const te::MidiClip*>(clip);
}

[[maybe_unused]] inline te::MidiClip*
isMidiClip(te::Clip& clip)
{
  return dynamic_cast<te::MidiClip*>(std::addressof(clip));
}

[[maybe_unused]] inline const te::MidiClip*
isMidiClip(const te::Clip& clip)
{
  return dynamic_cast<const te::MidiClip*>(std::addressof(clip));
}


// TODO: bump to track items

// Position

[[maybe_unused]] inline double getStart(te::Clip& clip)
{
  return clip.getPosition().getStart();
}

[[maybe_unused]] inline bool isOnLoopStart(te::Clip& clip)
{
  return getStart(clip) <= loopStartBeat;
}

[[maybe_unused]] inline double getEnd(te::Clip& clip)
{
  return clip.getPosition().getEnd();
}

[[maybe_unused]] inline bool isOnLoopEnd(te::Clip& clip)
{
  return getEnd(clip) >= loopEndBeat;
}

[[maybe_unused]] inline double getLength(te::Clip& clip)
{
  return clip.getPosition().getLength();
}


// Moving

[[maybe_unused]] inline void moveToLoopStartBeats(te::Clip& clip, double beats)
{
  auto& tempo = clip.getClipTrack()->edit.tempoSequence;
  clip.setPosition(
      {{loopStartBeat,
        tempo.beatsToTime(std::max(beats, loopEndBeat))}});
}

[[maybe_unused]] inline void moveToLoopStart(te::Clip& clip, double length)
{
  auto& tempo = clip.getClipTrack()->edit.tempoSequence;
  moveToLoopStartBeats(clip, tempo.timeToBeats(length));
}

[[maybe_unused]] inline void moveToLoopStart(te::Clip& clip)
{
  moveToLoopStartBeats(clip, clip.getLengthInBeats());
}


[[maybe_unused]] void setPosition(
    te::Clip& clip,
    double    start,
    double    end,
    double    offset = 0)
{
  clip.setPosition(
      {{start,
        end},
       offset});
}

[[maybe_unused]] void setPositionBeats(
    te::Clip& clip,
    double    startBeat,
    double    endBeat,
    double    offsetInBeats = 0)
{
  auto& tempo = clip.getClipTrack()->edit.tempoSequence;
  setPosition(
      clip,
      tempo.beatsToTime(startBeat),
      tempo.beatsToTime(endBeat),
      tempo.beatsToTime(offsetInBeats));
}


// Copy

// TODO: optimize?

[[maybe_unused]] te::Clip& copy(te::Clip& clip)
{
  auto& track = *clip.getClipTrack();

  auto copyState = clip.state.createCopy();

  track.edit.createNewItemID().writeID(copyState, nullptr);
  return *track.insertClipWithState(move(copyState));
}

BLOOPER_EXT_NAMESPACE_END

#endif // BLOOPER_EXT_CLIP_HPP