#ifndef BLOOPER_EDIT_EXT_HPP
#define BLOOPER_EDIT_EXT_HPP


BLOOPER_NAMESPACE_BEGIN

namespace ext::edit
{
static inline te::AudioTrack* getOrInsertAudioTrackAt(
        te::Edit& edit, int index)
{
    edit.ensureNumberOfAudioTracks(index + 1);
    return te::getAudioTracks(edit)[index];
}

static inline void togglePlay(te::Edit& edit)
{
    auto& transport = edit.getTransport();

    if (transport.isPlaying())
        transport.stop(false, false);
    else
        transport.play(false);
}

static inline void toggleRecord(te::Edit& edit)
{
    auto& transport = edit.getTransport();

    if (transport.isRecording())
        transport.stop(true, false);
    else
        transport.record(false);
}

static inline te::Clip::Ptr getLongestClip(const te::Edit& edit)
{
    te::Clip::Ptr longest;
    for (auto track : getClipTracks(edit))
        for (auto clip : track->getClips())
            if (clip->getLengthInBeats() > longest->getLengthInBeats())
                longest = clip;

    return longest;
}
} // namespace ext::edit

BLOOPER_NAMESPACE_END


#endif //BLOOPER_EDIT_EXT_HPP
