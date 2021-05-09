#ifndef BLOOPER_MAIN_COMPONENT_HPP
#define BLOOPER_MAIN_COMPONENT_HPP


#include <blooper/internal/internal.hpp>
#include <blooper/plugins/plugins.hpp>

#include <blooper/edit/EditComponent.hpp>
#include <blooper/toolbar/ToolbarComponent.hpp>
#include <blooper/browser/BrowserComponent.hpp>
#include <blooper/rack/RackComponent.hpp>


BLOOPER_NAMESPACE_BEGIN

class MainComponent final :
    public juce::Component,
    public juce::ChangeListener
{
public:
    MainComponent()
    {
        settingsButton.onClick = [this] {
            ext::engine::showAudioDeviceSettings(engine);
            createTracksAndAssignInputs();
        };

        pluginsButton.onClick = [this] {
            ext::engine::showPluginSettings(engine);
        };

        newEditButton.onClick = [this] {
            createOrLoadEdit();
        };

        updatePlayButtonText();
        updateRecordButtonText();
        editNameLabel.setJustificationType(juce::Justification::centred);


        addAndMakeVisible(settingsButton);
        addAndMakeVisible(pluginsButton);

        addAndMakeVisible(newEditButton);
        addAndMakeVisible(showEditButton);
        addAndMakeVisible(editNameLabel);

        addAndMakeVisible(playPauseButton);
        addAndMakeVisible(recordButton);

        addAndMakeVisible(newTrackButton);
        addAndMakeVisible(clearTracksButton);
        addAndMakeVisible(deleteButton);

        addAndMakeVisible(showWaveformButton);

        deleteButton.setEnabled(false);


        auto d = juce::File::getSpecialLocation(juce::File::tempDirectory)
                         .getChildFile("Blooper");
        d.createDirectory();

        auto f = utils::findRecentEdit(d);
        if (f.existsAsFile())
            createOrLoadEdit(f);
        else
            createOrLoadEdit(
                    d.getNonexistentChildFile(
                            "Test",
                            ".tracktionedit",
                            false));


        selectionManager.addChangeListener(this);

        setupButtons();

        setSize(700, 500);
    }

    ~MainComponent() override
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

        newEditButton.setBounds(
                topR.removeFromLeft(w).reduced(2));
        showEditButton.setBounds(
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

        topR = r.removeFromTop(30);
        showWaveformButton.setBounds(
                topR.removeFromLeft(w * 2)
                        .reduced(2));

        editNameLabel.setBounds(topR);

        if (editComponent != nullptr) editComponent->setBounds(r);
    }


private:
    tracktion_engine::Engine engine{
            JUCE_APPLICATION_NAME_STRING,
            std::make_unique<ExtendedUIBehaviour>(),
            nullptr};

    tracktion_engine::SelectionManager selectionManager{engine};

    std::unique_ptr<tracktion_engine::Edit> edit;
    std::unique_ptr<EditComponent>          editComponent;

    juce::TextButton
            settingsButton{"Settings"},
            pluginsButton{"Plugins"},
            newEditButton{"New"},
            playPauseButton{"Play"},
            recordButton{"Record"},
            showEditButton{"Show Edit"},
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

    void createOrLoadEdit(juce::File editFile = {})
    {
        if (editFile == juce::File())
        {
            juce::FileChooser fc(
                    "New Edit",
                    juce::File::getSpecialLocation(
                            juce::File::userDocumentsDirectory),
                    "*.tracktionedit");
            if (fc.browseForFileToSave(true))
                editFile = fc.getResult();
            else
                return;
        }

        selectionManager.deselectAll();
        editComponent = nullptr;

        if (editFile.existsAsFile())
            edit = tracktion_engine::loadEditFromFile(engine, editFile);
        else
            edit = tracktion_engine::createEmptyEdit(engine, editFile);

        edit->playInStopEnabled = true;

        auto& transport = edit->getTransport();
        transport.addChangeListener(this);

        editNameLabel.setText(
                editFile.getFileNameWithoutExtension(),
                juce::dontSendNotification);
        showEditButton.onClick = [this, editFile] {
            tracktion_engine::EditFileOperations(*edit)
                    .save(true,
                          true,
                          false);
            editFile.revealToUser();
        };

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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_MAIN_COMPONENT_HPP
