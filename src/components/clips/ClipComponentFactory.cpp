#include <blooper/components/clips/ClipComponentFactory.hpp>

BLOOPER_NAMESPACE_BEGIN

// Explicit instantiation

template class ClipComponentFactory<te::Clip>;

// Audio
template class ClipComponentFactory<te::AudioClipBase>;
template class ClipComponentFactory<te::WaveAudioClip>;

// Midi
template class ClipComponentFactory<te::MidiClip>;

BLOOPER_NAMESPACE_END
