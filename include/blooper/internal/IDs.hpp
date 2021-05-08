#ifndef BLOOPER_IDS_HPP
#define BLOOPER_IDS_HPP


namespace blooper::IDs
{
#define DECLARE_ID(name) \
    static inline const juce::Identifier name(#name); // NOLINT(cert-err58-cpp)

DECLARE_ID(EDITVIEWSTATE)
DECLARE_ID(showGlobalTrack)
DECLARE_ID(showMarkerTrack)
DECLARE_ID(showChordTrack)
DECLARE_ID(showMidiDevices)
DECLARE_ID(showWaveDevices)
DECLARE_ID(viewX1)
DECLARE_ID(viewX2)
DECLARE_ID(viewY)
DECLARE_ID(drawWaveforms)
DECLARE_ID(showHeaders)
DECLARE_ID(showFooters)
DECLARE_ID(showArranger)

#undef DECLARE_ID
} // namespace blooper::IDs


#endif //BLOOPER_IDS_HPP
