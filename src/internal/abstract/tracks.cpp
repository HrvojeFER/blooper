#include <blooper/internal/abstract/tracks.hpp>

BLOOPER_NAMESPACE_BEGIN

// Explicit instantiation

template class AnyAbstractTrackContentComponent<
    TrackContentTraits>;

template class AnyTrackContentComponentBase<
    TrackContentTraits,
    TrackTraits>;


template class AnyTrackContentComponentBase<
    TrackContentTraits,
    AudioTrackTraits>;

BLOOPER_NAMESPACE_END
