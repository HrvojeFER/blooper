#ifndef BLOOPER_EDIT_COMPONENT_HPP
#define BLOOPER_EDIT_COMPONENT_HPP


#include <blooper/internal/internal.hpp>

#include <blooper/edit/TrackComponents.hpp>
#include <blooper/edit/PlayheadComponent.hpp>


namespace blooper
{
using namespace juce;
namespace te = tracktion_engine;


class EditComponent :
    public Component,
    private te::ValueTreeAllEventListener,
    private FlaggedAsyncUpdater,
    private ChangeListener
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
            juce::ValueTree&, const juce::Identifier&) override;

    void valueTreeChildAdded(
            juce::ValueTree&, juce::ValueTree&) override;

    void valueTreeChildRemoved(
            juce::ValueTree&, juce::ValueTree&, int) override;

    void valueTreeChildOrderChanged(
            juce::ValueTree&, int, int) override;

    void handleAsyncUpdate() override;
    void resized() override;

    void changeListenerCallback(ChangeBroadcaster*) override
    {
        repaint();
    }


    void buildTracks();

    te::Edit& edit;

    EditViewState editViewState;

    PlayheadComponent                playhead{edit, editViewState};
    OwnedArray<TrackComponent>       tracks;
    OwnedArray<TrackHeaderComponent> headers;
    OwnedArray<TrackFooterComponent> footers;


    bool updateTracks = false,
         updateZoom = false;
};
} // namespace blooper


#endif // BLOOPER_EDIT_COMPONENT_HPP
