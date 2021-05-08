#ifndef BLOOPER_TRACK_COMPONENTS_HPP
#define BLOOPER_TRACK_COMPONENTS_HPP


#include <blooper/internal/internal.hpp>
#include <blooper/plugins/plugins.hpp>

#include <blooper/edit/ClipComponents.hpp>


namespace blooper
{
using namespace juce;
namespace te = tracktion_engine;


class TrackHeaderComponent :
    public Component,
    private te::ValueTreeAllEventListener
{
public:
    TrackHeaderComponent(EditViewState&, te::Track::Ptr);
    ~TrackHeaderComponent() override;


    void paint(Graphics& g) override;
    void mouseDown(const MouseEvent& e) override;
    void resized() override;


private:
    void valueTreeChanged() override { }

    void valueTreePropertyChanged(
            juce::ValueTree&,
            const juce::Identifier&) override;


    EditViewState& editViewState;
    te::Track::Ptr track;

    ValueTree inputsState;
    Label     trackName;

    TextButton
            armButton{"A"},
            muteButton{"M"},
            soloButton{"S"},
            inputButton{"I"};
};


class TrackFooterComponent :
    public Component,
    private FlaggedAsyncUpdater,
    private te::ValueTreeAllEventListener
{
public:
    TrackFooterComponent(EditViewState&, te::Track::Ptr);
    ~TrackFooterComponent() override;

    void paint(Graphics& g) override;
    void mouseDown(const MouseEvent& e) override;
    void resized() override;


private:
    void valueTreeChanged() override { }

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

    void buildPlugins();


    EditViewState& editViewState;
    te::Track::Ptr track;

    TextButton                  addButton{"+"};
    OwnedArray<PluginComponent> plugins;

    bool updatePlugins = false;
};


class TrackComponent :
    public Component,
    private te::ValueTreeAllEventListener,
    private FlaggedAsyncUpdater,
    private ChangeListener
{
public:
    TrackComponent(EditViewState&, te::Track::Ptr);
    ~TrackComponent() override;

    void paint(Graphics& g) override;
    void mouseDown(const MouseEvent& e) override;
    void resized() override;

private:
    void changeListenerCallback(ChangeBroadcaster*) override;

    void valueTreeChanged() override { }

    void valueTreePropertyChanged(
            juce::ValueTree&,
            const juce::Identifier&) override;

    void valueTreeChildAdded(
            juce::ValueTree&,
            juce::ValueTree&) override;

    void valueTreeChildRemoved(
            juce::ValueTree&,
            juce::ValueTree&, int) override;

    void valueTreeChildOrderChanged(
            juce::ValueTree&,
            int,
            int) override;

    void handleAsyncUpdate() override;

    void buildClips();
    void buildRecordClips();


    EditViewState& editViewState;
    te::Track::Ptr track;

    OwnedArray<ClipComponent>               clips;
    std::unique_ptr<RecordingClipComponent> recordingClip;

    bool updateClips = false,
         updatePositions = false,
         updateRecordClips = false;
};
} // namespace blooper


#endif // BLOOPER_TRACK_COMPONENTS_HPP
