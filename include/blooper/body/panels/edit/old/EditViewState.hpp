#ifndef BLOOPER_OLD_EDIT_VIEW_STATE_HPP
#define BLOOPER_OLD_EDIT_VIEW_STATE_HPP


#include "../../../../internal/internal.hpp"
#include "../../../../context/context.hpp"


BLOOPER_NAMESPACE_BEGIN

class EditViewState
{
 public:
  EditViewState(te::Edit& e, te::SelectionManager& s)
      : edit(e),
        selectionManager(s)
  {
    state = edit.state.getOrCreateChildWithName(
        id::editViewState,
        nullptr);

    auto um = &edit.getUndoManager();


    showGlobalTrack.referTo(
        state,
        id::showGlobalTrack,
        um,
        false);
    showMarkerTrack.referTo(
        state,
        id::showMarkerTrack,
        um,
        false);

    showChordTrack.referTo(
        state,
        id::showChordTrack,
        um,
        false);

    showArrangerTrack.referTo(
        state,
        id::showArranger,
        um,
        false);

    drawWaveforms.referTo(
        state,
        id::drawWaveforms,
        um,
        true);

    showHeaders.referTo(
        state,
        id::showHeaders,
        um,
        true);

    showFooters.referTo(
        state,
        id::showFooters,
        um,
        false);

    showMidiDevices.referTo(
        state,
        id::showMidiDevices,
        um,
        false);

    showWaveDevices.referTo(
        state,
        id::showWaveDevices,
        um,
        true);


    viewX1.referTo(
        state,
        id::viewX1,
        um,
        0);

    viewX2.referTo(
        state,
        id::viewX2,
        um,
        60);

    viewY.referTo(
        state,
        id::viewY,
        um,
        0);


    bpm.referTo(
        state,
        id::bpm,
        um,
        120.0);
  }


  [[nodiscard]] int timeToX(double time, int width) const
  {
    return juce::roundToInt(
        ((time - viewX1) * width) /
        (viewX2 - viewX1));
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

  juce::CachedValue<bool>
      showGlobalTrack,
      showMarkerTrack,
      showChordTrack,
      showArrangerTrack,
      drawWaveforms,
      showHeaders,
      showFooters,
      showMidiDevices,
      showWaveDevices;

  juce::CachedValue<double>
      viewX1,
      viewX2,
      viewY,

      bpm;

  juce::ValueTree state;
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_OLD_EDIT_VIEW_STATE_HPP
