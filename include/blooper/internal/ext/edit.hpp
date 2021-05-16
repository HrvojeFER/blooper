#ifndef BLOOPER_EXT_EDIT_HPP
#define BLOOPER_EXT_EDIT_HPP


BLOOPER_EXT_NAMESPACE_BEGIN

namespace edit
{
namespace
{
inline te::AudioTrack* getOrInsertAudioTrackAt(
        te::Edit& edit, int index)
{
    edit.ensureNumberOfAudioTracks(index + 1);
    return te::getAudioTracks(edit)[index];
}

inline void togglePlay(te::Edit& edit)
{
    auto& transport = edit.getTransport();

    if (transport.isPlaying())
        transport.stop(false, false);
    else
        transport.play(false);
}

inline void toggleRecord(te::Edit& edit)
{
    auto& transport = edit.getTransport();

    if (transport.isRecording())
        transport.stop(true, false);
    else
        transport.record(false);
}

inline te::Clip::Ptr getLongestClip(const te::Edit& edit)
{
    te::Clip::Ptr longest;
    for (auto track : getClipTracks(edit))
        for (auto clip : track->getClips())
            if (clip->getLengthInBeats() > longest->getLengthInBeats())
                longest = clip;

    return longest;
}
} // namespace
} // namespace edit

BLOOPER_EXT_NAMESPACE_END


#endif // BLOOPER_EXT_EDIT_HPP
