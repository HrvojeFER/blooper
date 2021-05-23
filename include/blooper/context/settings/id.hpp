#ifndef BLOOPER_ID_HPP
#define BLOOPER_ID_HPP

#include <blooper/internal/abstract/abstract.hpp>

BLOOPER_ID_NAMESPACE_BEGIN

#if __JETBRAINS_IDE__
    #pragma clang diagnostic push
    #pragma ide diagnostic   ignored "cert-err58-cpp"
#endif


static inline const juce::Identifier editViewState("editViewState");

static inline const juce::Identifier showGlobalTrack("showGlobalTrack");
static inline const juce::Identifier showMarkerTrack("showMarkerTrack");
static inline const juce::Identifier showChordTrack("showChordTrack");
static inline const juce::Identifier showMidiDevices("showMidiDevices");
static inline const juce::Identifier showWaveDevices("showWaveDevices");

static inline const juce::Identifier drawWaveforms("drawWaveforms");
static inline const juce::Identifier showHeaders("showHeaders");
static inline const juce::Identifier showFooters("showFooters");
static inline const juce::Identifier showArranger("showArranger");

static inline const juce::Identifier viewX1("viewX1");
static inline const juce::Identifier viewX2("viewX2");
static inline const juce::Identifier viewY("viewY");

static inline const juce::Identifier bpm("bpm");


#if __JETBRAINS_IDE__
    #pragma clang diagnostic pop
#endif

BLOOPER_ID_NAMESPACE_END

#endif // BLOOPER_ID_HPP
