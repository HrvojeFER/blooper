#ifndef BLOOPER_EXT_EDIT_HPP
#define BLOOPER_EXT_EDIT_HPP

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/abstract.hpp>

BLOOPER_EXT_NAMESPACE_BEGIN

[[maybe_unused]] inline te::AudioTrack* getOrInsertAudioTrackAt(
    te::Edit& edit,
    int       index)
{
  edit.ensureNumberOfAudioTracks(index + 1);
  return te::getAudioTracks(edit)[index];
}

[[maybe_unused]] inline void togglePlay(te::Edit& edit)
{
  auto& transport = edit.getTransport();

  if (transport.isPlaying())
    transport.stop(false, false);
  else
    transport.play(false);
}

[[maybe_unused]] inline void toggleRecord(te::Edit& edit)
{
  auto& transport = edit.getTransport();

  if (transport.isRecording())
    transport.stop(true, false);
  else
    transport.record(false);
}

[[maybe_unused]] inline te::Clip::Ptr getLongestClip(const te::Edit& edit)
{
  te::Clip::Ptr longest;
  for (auto track : getClipTracks(edit))
    for (auto clip : track->getClips())
      if (clip->getLengthInBeats() > longest->getLengthInBeats())
        longest = clip;

  return longest;
}

BLOOPER_EXT_NAMESPACE_END

#endif // BLOOPER_EXT_EDIT_HPP
