#ifndef BLOOPER_EDIT_VIEW_STATE_HPP
#define BLOOPER_EDIT_VIEW_STATE_HPP


#include <blooper/internal/utilities.hpp>

#include <blooper/internal/IDs.hpp>


namespace blooper
{
using namespace juce;
namespace te = tracktion_engine;


class EditViewState
{
public:
    EditViewState(te::Edit& e, te::SelectionManager& s)
        : edit(e),
          selectionManager(s)
    {
        state = edit.state.getOrCreateChildWithName(
                IDs::EDITVIEWSTATE,
                nullptr);

        auto um = &edit.getUndoManager();


        showGlobalTrack.referTo(
                state,
                IDs::showGlobalTrack,
                um,
                false);
        showMarkerTrack.referTo(
                state,
                IDs::showMarkerTrack,
                um,
                false);

        showChordTrack.referTo(
                state,
                IDs::showChordTrack,
                um,
                false);

        showArrangerTrack.referTo(
                state,
                IDs::showArranger,
                um,
                false);

        drawWaveforms.referTo(
                state,
                IDs::drawWaveforms,
                um,
                true);

        showHeaders.referTo(
                state,
                IDs::showHeaders,
                um,
                true);

        showFooters.referTo(
                state,
                IDs::showFooters,
                um,
                false);

        showMidiDevices.referTo(
                state,
                IDs::showMidiDevices,
                um,
                false);

        showWaveDevices.referTo(
                state,
                IDs::showWaveDevices,
                um,
                true);


        viewX1.referTo(
                state,
                IDs::viewX1,
                um,
                0);

        viewX2.referTo(
                state,
                IDs::viewX2,
                um,
                60);

        viewY.referTo(
                state,
                IDs::viewY,
                um,
                0);
    }


    [[nodiscard]] int timeToX(double time, int width) const
    {
        return roundToInt(((time - viewX1) * width) / (viewX2 - viewX1));
    }

    [[nodiscard]] double xToTime(int x, int width) const
    {
        return (double(x) / width) * (viewX2 - viewX1) + viewX1;
    }

    [[nodiscard]] double beatToTime(double b) const
    {
        auto& ts = edit.tempoSequence;
        return ts.beatsToTime(b);
    }


    te::Edit&             edit;
    te::SelectionManager& selectionManager;

    CachedValue<bool>
            showGlobalTrack,
            showMarkerTrack,
            showChordTrack,
            showArrangerTrack,
            drawWaveforms,
            showHeaders,
            showFooters,
            showMidiDevices,
            showWaveDevices;

    CachedValue<double>
            viewX1,
            viewX2,
            viewY;

    ValueTree state;
};
} // namespace blooper


#endif // BLOOPER_EDIT_VIEW_STATE_HPP
