#include <blooper/internal/abstract/plugins.hpp>

BLOOPER_NAMESPACE_BEGIN

// Explicit instantiation

// content

template class AnyAbstractPluginContentComponent<
    PluginContentTraits>;

template class AnyPluginContentComponentBase<
    PluginContentTraits>;

template class AnyPluginContentComponentBase<
    PluginContentTraits,
    ExternalPluginTraits>;

// effects

template class AnyPluginContentComponentBase<
    PluginContentTraits,
    AnyPluginTraits<te::VolumeAndPanPlugin>>;

template class AnyPluginContentComponentBase<
    PluginContentTraits,
    AnyPluginTraits<te::EqualiserPlugin>>;

// TODO: fix
//template class AnyPluginContentComponentBase<
//    PluginContentTraits,
//    AnyPluginTraits<te::ReverbPlugin>>;

template class AnyPluginContentComponentBase<
    PluginContentTraits,
    AnyPluginTraits<te::DelayPlugin>>;

template class AnyPluginContentComponentBase<
    PluginContentTraits,
    AnyPluginTraits<te::ChorusPlugin>>;

template class AnyPluginContentComponentBase<
    PluginContentTraits,
    AnyPluginTraits<te::PhaserPlugin>>;

template class AnyPluginContentComponentBase<
    PluginContentTraits,
    AnyPluginTraits<te::CompressorPlugin>>;

template class AnyPluginContentComponentBase<
    PluginContentTraits,
    AnyPluginTraits<te::PitchShiftPlugin>>;

template class AnyPluginContentComponentBase<
    PluginContentTraits,
    AnyPluginTraits<te::LowPassPlugin>>;

template class AnyPluginContentComponentBase<
    PluginContentTraits,
    AnyPluginTraits<te::MidiModifierPlugin>>;

template class AnyPluginContentComponentBase<
    PluginContentTraits,
    AnyPluginTraits<te::MidiPatchBayPlugin>>;

template class AnyPluginContentComponentBase<
    PluginContentTraits,
    AnyPluginTraits<te::PatchBayPlugin>>;

template class AnyPluginContentComponentBase<
    PluginContentTraits,
    AnyPluginTraits<te::AuxSendPlugin>>;

template class AnyPluginContentComponentBase<
    PluginContentTraits,
    AnyPluginTraits<te::AuxReturnPlugin>>;

template class AnyPluginContentComponentBase<
    PluginContentTraits,
    AnyPluginTraits<te::TextPlugin>>;

template class AnyPluginContentComponentBase<
    PluginContentTraits,
    AnyPluginTraits<te::FreezePointPlugin>>;

template class AnyPluginContentComponentBase<
    PluginContentTraits,
    AnyPluginTraits<te::InsertPlugin>>;

// synths

// TODO: fix
//template class AnyPluginContentComponentBase<
//    PluginContentTraits,
//    AnyPluginTraits<te::FourOscPlugin>>;

template class AnyPluginContentComponentBase<
    PluginContentTraits,
    AnyPluginTraits<te::SamplerPlugin>>;

BLOOPER_NAMESPACE_END
