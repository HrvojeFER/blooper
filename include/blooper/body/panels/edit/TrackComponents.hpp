#ifndef BLOOPER_TRACK_COMPONENTS_HPP
#define BLOOPER_TRACK_COMPONENTS_HPP


#include <blooper/internal/internal.hpp>
#include <blooper/engine/engine.hpp>

#include <blooper/body/panels/edit/EditViewState.hpp>
#include <blooper/body/panels/edit/ClipComponents.hpp>


BLOOPER_NAMESPACE_BEGIN

class TrackHeaderComponent :
    public juce::Component,
    private te::ValueTreeAllEventListener
{
public:
    TrackHeaderComponent(EditViewState&, te::Track::Ptr);
    ~TrackHeaderComponent() override;


    void mouseDown(const juce::MouseEvent& e) override;

    void paint(juce::Graphics& g) override;
    void resized() override;


private:
    void valueTreeChanged() override { }

    void valueTreePropertyChanged(
            juce::ValueTree&,
            const juce::Identifier&) override;


    EditViewState& editViewState;
    te::Track::Ptr track;

    juce::ValueTree inputsState;
    juce::Label     trackName;

    juce::TextButton
            addClipButton{"C"},
            armButton{"A"},
            muteButton{"M"},
            soloButton{"S"},
            inputButton{"I"};
};


class TrackFooterComponent :
    public juce::Component,
    private utils::FlaggedAsyncUpdater,
    private te::ValueTreeAllEventListener
{
public:
    TrackFooterComponent(EditViewState&, te::Track::Ptr);
    ~TrackFooterComponent() override;

    void paint(juce::Graphics& g) override;
    void mouseDown(const juce::MouseEvent& e) override;
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

    juce::TextButton                  addButton{"+"};
    juce::OwnedArray<PluginComponent> plugins;

    bool updatePlugins = false;
};


class TrackComponent :
    public juce::Component,
    private te::ValueTreeAllEventListener,
    private utils::FlaggedAsyncUpdater,
    private juce::ChangeListener
{
public:
    TrackComponent(EditViewState&, te::Track::Ptr);
    ~TrackComponent() override;

    void paint(juce::Graphics& g) override;
    void mouseDown(const juce::MouseEvent& e) override;
    void resized() override;

private:
    void changeListenerCallback(juce::ChangeBroadcaster*) override;

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

    juce::OwnedArray<ClipComponent>         clips;
    std::unique_ptr<RecordingClipComponent> recordingClip;

    bool updateClips = false,
         updatePositions = false,
         updateRecordClips = false;
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_TRACK_COMPONENTS_HPP
