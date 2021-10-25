#include <blooper/internal/abstract/clips.hpp>

BLOOPER_NAMESPACE_BEGIN

// Explicit instantiation

template class AnyAbstractClipContentComponent<ClipContentTraits>;
template class AnyClipContentComponentBase<ClipContentTraits>;

template class AnyAbstractClipContentComponent<AudioClipContentTraits>;
template class AnyClipContentComponentBase<AudioClipContentTraits>;

template class AnyClipContentComponentBase<WaveAudioClipContentTraits>;

template class AnyClipContentComponentBase<MidiClipContentTraits>;

BLOOPER_NAMESPACE_END
