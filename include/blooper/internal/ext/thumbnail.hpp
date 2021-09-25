#ifndef BLOOPER_EXT_THUMBNAIL_HPP
#define BLOOPER_EXT_THUMBNAIL_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>

#include <blooper/internal/abstract/juceTraits.hpp>

BLOOPER_EXT_NAMESPACE_BEGIN

[[maybe_unused, nodiscard]] inline JuceRecordingThumbnailRef
getRecordingThumbnail(const te::AudioTrack& audioTrack)
{
  auto& edit = audioTrack.edit;
  auto& engine = edit.engine;

  for (auto* device :
       edit.getEditInputDevices()
           .getDevicesForTargetTrack(audioTrack))
  {
    if (device->getRecordingFile().exists())
    {
      return engine
          .getRecordingThumbnailManager()
          .getThumbnailFor(device->getRecordingFile());
    }
  }

  return {};
}

[[maybe_unused, nodiscard]] inline JuceTimeRange
getTimeRange(const JuceRecordingThumbnail& thumb,
             const JuceEdit&               edit) noexcept
{
  auto& transport = edit.getTransport();

  TimePixelMapping result{};

  const auto timeStarted = thumb.punchInTime;
  const auto unloopedPosition = timeStarted + thumb.thumb.getTotalLength();
  const auto loopRange = transport.getLoopRange();

  auto start = timeStarted;
  auto end = unloopedPosition;

  if (transport.looping && end >= loopRange.end)
  {
    start = std::min(start, loopRange.start);
    end = transport.position;
  }
  else if (edit.recordingPunchInOut)
  {
    const double in = thumb.punchInTime;
    const double out = edit.getTransport().getLoopRange().getEnd();

    start = std::clamp(start, in, out);
    end = std::clamp(end, in, out);
  }

  return {start, end};
}

BLOOPER_EXT_NAMESPACE_END

#endif // BLOOPER_EXT_THUMBNAIL_HPP
