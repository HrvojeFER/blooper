#ifndef BLOOPER_EXT_TRACK_HPP
#define BLOOPER_EXT_TRACK_HPP

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/abstract.hpp>

BLOOPER_EXT_NAMESPACE_BEGIN

[[maybe_unused]] inline void armTrack(
    te::AudioTrack& t,
    bool            arm,
    int             position = 0)
{
  auto& edit = t.edit;
  for (auto instance : edit.getAllInputDevices())
    if (instance->isOnTargetTrack(t, position))
      instance->setRecordingEnabled(t, arm);
}

[[maybe_unused]] inline bool isTrackArmed(
    te::AudioTrack& t,
    int             position = 0)
{
  auto& edit = t.edit;
  for (auto instance : edit.getAllInputDevices())
    if (instance->isOnTargetTrack(t, position))
      return instance->isRecordingEnabled(t);

  return false;
}

[[maybe_unused]] inline bool isInputMonitoringEnabled(
    te::AudioTrack& t,
    int             position = 0)
{
  auto& edit = t.edit;
  for (auto instance : edit.getAllInputDevices())
    if (instance->isOnTargetTrack(t, position))
      return instance->getInputDevice().isEndToEndEnabled();

  return false;
}

[[maybe_unused]] inline void enableInputMonitoring(
    te::AudioTrack& t,
    bool            im,
    int             position = 0)
{
  if (isInputMonitoringEnabled(t, position) != im)
  {
    auto& edit = t.edit;
    for (auto instance : edit.getAllInputDevices())
      if (instance->isOnTargetTrack(t, position))
        instance->getInputDevice().flipEndToEnd();
  }
}

[[maybe_unused]] inline bool trackHasInput(te::AudioTrack& t, int position = 0)
{
  auto& edit = t.edit;
  for (auto instance : edit.getAllInputDevices())
    if (instance->isOnTargetTrack(t, position))
      return true;

  return false;
}

[[maybe_unused]] inline void loopClips(
    const te::ClipTrack& track,
    te::EditTimeRange    range)
{
  for (auto clip : track.getClips())
    clip->setLoopRange(range);
}

[[maybe_unused]] inline void extendClips(
    const te::ClipTrack& track,
    double               to)
{
  for (auto clip : track.getClips())
    clip->setLength(to, true);
}

BLOOPER_EXT_NAMESPACE_END

#endif // BLOOPER_EXT_TRACK_HPP
