#ifndef BLOOPER_BODY_COMPONENT_OLD_HPP
#define BLOOPER_BODY_COMPONENT_OLD_HPP


#include <blooper/internal/internal.hpp>
#include <blooper/style/style.hpp>
#include <blooper/context/context.hpp>

#include <blooper/body/panels/panels.hpp>

#include <blooper/body/header/header.hpp>
#include <blooper/body/footer/footer.hpp>


BLOOPER_NAMESPACE_BEGIN

class BodyComponentOld final :
    public juce::Component,
    public juce::ChangeListener,
    private juce::Timer
{
public:
    BodyComponentOld()
    {
        addAndMakeVisible(settingsButton);
        settingsButton.onClick = [this] {
            ext::engine::showAudioDeviceSettings(engine);
            createTracksAndAssignInputs();
        };

        addAndMakeVisible(pluginsButton);
        pluginsButton.onClick = [this] {
            ext::engine::showPluginSettings(engine);
        };

        addAndMakeVisible(playPauseButton);
        updatePlayButtonText();

        addAndMakeVisible(recordButton);
        updateRecordButtonText();

        addAndMakeVisible(deleteButton);
        deleteButton.setEnabled(false);

        addAndMakeVisible(newTrackButton);
        addAndMakeVisible(clearTracksButton);

        addAndMakeVisible(browser);
        browser.onFileSelected = [this](const juce::File& file) {
            auto sel = selectionManager.getSelectedObject(0);
            if (auto track = dynamic_cast<te::AudioTrack*>(sel))
                if (file.getFileExtension() == ".wav")
                    track->insertWaveClip(
                            file.getFileNameWithoutExtension(),
                            file,
                            {{0, 1.0}},
                            false);
        };

        addAndMakeVisible(showWaveformButton);

        setupButtons();
        selectionManager.addChangeListener(this);
        startTimerHz(30);
        setSize(700, 500);
    }

    ~BodyComponentOld() override
    {
        tracktion_engine::EditFileOperations(*edit)
                .save(true,
                      true,
                      false);

        engine.getTemporaryFileManager()
                .getTempDirectory()
                .deleteRecursively();
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(
                getLookAndFeel().findColour(
                        juce::ResizableWindow::backgroundColourId));
    }

    void resized() override
    {
        auto r = getLocalBounds();
        int  w = r.getWidth() / 9;
        auto topR = r.removeFromTop(30);

        settingsButton.setBounds(
                topR.removeFromLeft(w).reduced(2));
        pluginsButton.setBounds(
                topR.removeFromLeft(w).reduced(2));

        playPauseButton.setBounds(
                topR.removeFromLeft(w).reduced(2));
        recordButton.setBounds(
                topR.removeFromLeft(w).reduced(2));

        newTrackButton.setBounds(
                topR.removeFromLeft(w).reduced(2));
        clearTracksButton.setBounds(
                topR.removeFromLeft(w).reduced(2));
        deleteButton.setBounds(
                topR.removeFromLeft(w).reduced(2));

        browser.setBounds(
                r.removeFromLeft(
                        int(r.getWidth() * 0.2)));

        topR = r.removeFromTop(30);
        showWaveformButton.setBounds(
                topR.removeFromLeft(w * 2)
                        .reduced(2));
        if (bpmProperty != nullptr)
            bpmProperty->setBounds(
                    topR.removeFromLeft(w * 2)
                            .reduced(2));

        if (editComponent != nullptr) editComponent->setBounds(r);
    }


private:
    tracktion_engine::Engine engine{
            JUCE_APPLICATION_NAME_STRING,
            std::make_unique<UIBehaviour>(),
            nullptr};

    tracktion_engine::SelectionManager selectionManager{engine};

    std::unique_ptr<tracktion_engine::Edit> edit;
    std::unique_ptr<EditComponent>          editComponent;


    std::unique_ptr<juce::Value>                   bpmValue;
    std::unique_ptr<juce::SliderPropertyComponent> bpmProperty;
    double                                         bpm = 0;


    BrowserComponent browser;

    juce::TextButton
            settingsButton{"Settings"},
            pluginsButton{"Plugins"},
            playPauseButton{"Play"},
            recordButton{"Record"},
            newTrackButton{"New Track"},
            clearTracksButton{"Clear Tracks"},
            deleteButton{"Delete"};

    juce::Label        editNameLabel{"No Edit Loaded"};
    juce::ToggleButton showWaveformButton{"Show Waveforms"};


    void setupButtons()
    {
        playPauseButton.onClick = [this] {
            bool wasRecording = edit->getTransport().isRecording();
            ext::edit::togglePlay(*edit);
            if (wasRecording)
                tracktion_engine::EditFileOperations(*edit)
                        .save(true,
                              true,
                              false);
        };

        recordButton.onClick = [this] {
            bool wasRecording = edit->getTransport().isRecording();
            ext::edit::toggleRecord(*edit);
            if (wasRecording)
                tracktion_engine::EditFileOperations(*edit)
                        .save(true,
                              true,
                              false);
        };

        newTrackButton.onClick = [this] {
            edit->ensureNumberOfAudioTracks(
                    getAudioTracks(*edit).size() + 1);
        };

        clearTracksButton.onClick = [this] {
            for (auto t : tracktion_engine::getAudioTracks(*edit))
                edit->deleteTrack(t);
        };

        deleteButton.onClick = [this] {
            auto sel = selectionManager.getSelectedObject(0);
            if (auto clip = dynamic_cast<te::Clip*>(sel))
            {
                clip->removeFromParentTrack();
            }
            else if (auto track = dynamic_cast<te::Track*>(sel))
            {
                if (!(track->isMarkerTrack() ||
                      track->isTempoTrack() ||
                      track->isChordTrack()))
                    edit->deleteTrack(track);
            }
            else if (auto plugin = dynamic_cast<te::Plugin*>(sel))
            {
                plugin->deleteFromParent();
            }
        };

        showWaveformButton.onClick = [this] {
            auto& evs = editComponent->getEditViewState();
            evs.drawWaveforms = !evs.drawWaveforms.get();
            showWaveformButton.setToggleState(
                    evs.drawWaveforms,
                    juce::dontSendNotification);
        };
    }

    void updatePlayButtonText()
    {
        if (edit != nullptr)
            playPauseButton.setButtonText(
                    edit->getTransport().isPlaying() ?
                            "Stop" :
                            "Play");
    }

    void updateRecordButtonText()
    {
        if (edit != nullptr)
            recordButton.setButtonText(
                    edit->getTransport().isRecording() ?
                            "Abort" :
                            "Record");
    }

    void setupEdit(te::Project& project)
    {
        edit = ext::project::ensureEdit(project, engine);
        edit->playInStopEnabled = true;

        auto& transport = edit->getTransport();
        transport.addChangeListener(this);
        transport.looping = true;

        createTracksAndAssignInputs();
        tracktion_engine::EditFileOperations(*edit)
                .save(true,
                      true,
                      false);

        editComponent = std::make_unique<EditComponent>(
                *edit, selectionManager);
        editComponent->getEditViewState().showFooters = true;
        editComponent->getEditViewState().showMidiDevices = true;
        editComponent->getEditViewState().showWaveDevices = true;
        addAndMakeVisible(*editComponent);

        bpmValue = std::make_unique<juce::Value>(
                editComponent->getEditViewState().bpm.getPropertyAsValue());
        bpmProperty = std::make_unique<juce::SliderPropertyComponent>(
                *bpmValue,
                "bpm",
                0.0,
                200.0,
                0.01);
        addAndMakeVisible(*bpmProperty);

        resized();
    }

    void createTracksAndAssignInputs()
    {
        auto& dm = engine.getDeviceManager();

        for (int i = 0; i < dm.getNumWaveInDevices(); i++)
            if (auto wip = dm.getWaveInDevice(i))
                wip->setStereoPair(false);

        for (int i = 0; i < dm.getNumWaveInDevices(); i++)
        {
            if (auto wip = dm.getWaveInDevice(i))
            {
                wip->setEndToEnd(true);
                wip->setEnabled(true);
            }
        }

        edit->getTransport().ensureContextAllocated();


        int trackNum = 0;
        for (auto instance : edit->getAllInputDevices())
        {
            if (instance->getInputDevice().getDeviceType() ==
                tracktion_engine::InputDevice::waveDevice)
            {
                if (auto t = ext::edit::getOrInsertAudioTrackAt(
                            *edit, trackNum))
                {
                    instance->setTargetTrack(*t, 0, true);
                    instance->setRecordingEnabled(*t, true);

                    trackNum++;
                }
            }
        }

        edit->restartPlayback();
    }


    void changeListenerCallback(juce::ChangeBroadcaster* source) override
    {
        if (edit != nullptr && source == &edit->getTransport())
        {
            updatePlayButtonText();
            updateRecordButtonText();
        }
        else if (source == &selectionManager)
        {
            auto sel = selectionManager.getSelectedObject(0);
            deleteButton.setEnabled(
                    dynamic_cast<te::Clip*>(sel) != nullptr ||
                    dynamic_cast<te::Track*>(sel) != nullptr ||
                    dynamic_cast<te::Plugin*>(sel));
        }
    }

    void timerCallback() override
    {
        auto newBpm = double(bpmValue->getValue());
        if (newBpm != bpm)
        {
            edit->getTransport().setLoopRange(
                    {0, 60 / newBpm * 4});
            bpm = newBpm;
        }
    }


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BodyComponentOld)
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_BODY_COMPONENT_OLD_HPP
