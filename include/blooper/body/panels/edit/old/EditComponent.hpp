#ifndef BLOOPER_OLD_EDIT_PANEL_COMPONENT_HPP
#define BLOOPER_OLD_EDIT_PANEL_COMPONENT_HPP


#include "../../../../internal/internal.hpp"

#include "EditViewState.hpp"
#include "TrackComponents.hpp"
#include "PlayheadComponent.hpp"


BLOOPER_NAMESPACE_BEGIN

class EditComponent :
    public juce::Component,
    private te::ValueTreeAllEventListener,
    private utils::FlaggedAsyncUpdater,
    private juce::ChangeListener
{
 public:
  EditComponent(te::Edit&, te::SelectionManager&);
  ~EditComponent() override;

  EditViewState& getEditViewState()
  {
    return editViewState;
  }


 private:
  void valueTreeChanged() override { }

  void valueTreePropertyChanged(
      juce::ValueTree&,
      const juce::Identifier&) override;

  void valueTreeChildAdded(
      juce::ValueTree&,
      juce::ValueTree&) override;

  void valueTreeChildRemoved(
      juce::ValueTree&,
      juce::ValueTree&,
      int) override;

  void valueTreeChildOrderChanged(
      juce::ValueTree&,
      int,
      int) override;

  void handleAsyncUpdate() override;
  void resized() override;

  void changeListenerCallback(juce::ChangeBroadcaster*) override
  {
    repaint();
  }


  void buildTracks();

  te::Edit& edit;

  EditViewState editViewState;

  PlayheadComponent                      playhead{edit, editViewState};
  juce::OwnedArray<TrackComponent>       tracks;
  juce::OwnedArray<TrackHeaderComponent> headers;
  juce::OwnedArray<TrackFooterComponent> footers;


  bool updateTracks = false,
       updateZoom = false;
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_OLD_EDIT_PANEL_COMPONENT_HPP
