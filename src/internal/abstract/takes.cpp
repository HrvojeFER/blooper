#include <blooper/internal/abstract/takes.hpp>

BLOOPER_NAMESPACE_BEGIN

// Explicit instantiation

template class AnyAbstractTakeContentComponent<
    AbstractTakeContentTraits>;


template class AnyTakeContentComponentBase<
    AbstractTakeContentTraits,
    WaveAudioTakeTraits>;

template class AnyTakeContentComponentBase<
    AbstractTakeContentTraits,
    MidiTakeTraits>;

BLOOPER_NAMESPACE_END
