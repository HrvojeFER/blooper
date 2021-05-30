#include <blooper/body/panels/edit/old/TrackComponents.hpp>


BLOOPER_NAMESPACE_BEGIN

TrackHeaderComponent::TrackHeaderComponent(
        EditViewState& evs,
        te::Track::Ptr t)
    : editViewState(evs), track(move(t))
{
    utils::addAndMakeVisible(
            *this,
            {&addClipButton,
             &trackName,
             &armButton,
             &muteButton,
             &soloButton,
             &inputButton});

    armButton.setColour(
            juce::TextButton::buttonOnColourId,
            juce::Colours::red);
    muteButton.setColour(
            juce::TextButton::buttonOnColourId,
            juce::Colours::red);
    soloButton.setColour(
            juce::TextButton::buttonOnColourId,
            juce::Colours::green);

    trackName.setText(
            track->getName(),
            juce::dontSendNotification);
    trackName.setEditable(true);
    trackName.onTextChange = [this] { track->setName(trackName.getText()); };

    if (auto at = dynamic_cast<te::AudioTrack*>(track.get()))
    {
        inputButton.onClick = [this, at] {
            juce::PopupMenu m;

            if (ext::track::trackHasInput(*at))
            {
                bool ticked = ext::track::isInputMonitoringEnabled(*at);
                m.addItem(
                        1000,
                        "Input Monitoring",
                        true,
                        ticked);
                m.addSeparator();
            }

            if (editViewState.showWaveDevices)
            {
                int id = 1;
                for (auto instance : at->edit.getAllInputDevices())
                {
                    if (instance->getInputDevice().getDeviceType() ==
                        te::InputDevice::waveDevice)
                    {
                        bool ticked = instance->getTargetTracks().getFirst() ==
                                      at;
                        m.addItem(
                                id++,
                                instance->getInputDevice().getName(),
                                true,
                                ticked);
                    }
                }
            }

            if (editViewState.showMidiDevices)
            {
                m.addSeparator();

                int id = 100;
                for (auto instance : at->edit.getAllInputDevices())
                {
                    if (instance->getInputDevice().getDeviceType() ==
                        te::InputDevice::physicalMidiDevice)
                    {
                        bool ticked = instance->getTargetTracks().getFirst() ==
                                      at;
                        m.addItem(
                                id++,
                                instance->getInputDevice().getName(),
                                true,
                                ticked);
                    }
                }
            }

            int res = m.show();

            if (res == 1000)
            {
                ext::track::enableInputMonitoring(
                        *at,
                        !ext::track::isInputMonitoringEnabled(*at));
            }
            else if (res >= 100)
            {
                int id = 100;
                for (auto instance : at->edit.getAllInputDevices())
                {
                    if (instance->getInputDevice().getDeviceType() ==
                        te::InputDevice::physicalMidiDevice)
                    {
                        if (id == res)
                            instance->setTargetTrack(
                                    *at,
                                    0,
                                    true);
                        id++;
                    }
                }
            }
            else if (res >= 1)
            {
                int id = 1;
                for (auto instance : at->edit.getAllInputDevices())
                {
                    if (instance->getInputDevice().getDeviceType() ==
                        te::InputDevice::waveDevice)
                    {
                        if (id == res)
                            instance->setTargetTrack(
                                    *at,
                                    0,
                                    true);
                        id++;
                    }
                }
            }
        };
        armButton.onClick = [this, at] {
            ext::track::armTrack(
                    *at,
                    !ext::track::isTrackArmed(*at));
            armButton.setToggleState(
                    ext::track::isTrackArmed(*at),
                    juce::dontSendNotification);
        };
        addClipButton.onClick = [at] {
            ext::track::showClipLoader(*at);
        };


        muteButton.onClick = [at] {
            at->setMute(!at->isMuted(false));
        };
        soloButton.onClick = [at] {
            at->setSolo(!at->isSolo(false));
        };

        armButton.setToggleState(
                ext::track::isTrackArmed(*at),
                juce::dontSendNotification);
    }
    else
    {
        armButton.setVisible(false);
        muteButton.setVisible(false);
        soloButton.setVisible(false);
    }

    track->state.addListener(this);
    inputsState = track->edit.state.getChildWithName(te::IDs::INPUTDEVICES);
    inputsState.addListener(this);

    // this is JUCE code...
    valueTreePropertyChanged(track->state, te::IDs::mute);
    valueTreePropertyChanged(track->state, te::IDs::solo);
    valueTreePropertyChanged(inputsState, te::IDs::targetIndex);
}

TrackHeaderComponent::~TrackHeaderComponent()
{
    track->state.removeListener(this);
}

void TrackHeaderComponent::valueTreePropertyChanged(
        juce::ValueTree&        v,
        const juce::Identifier& i)
{
    if (te::TrackList::isTrack(v))
    {
        if (i == te::IDs::mute)
            muteButton.setToggleState(
                    (bool) v[i],
                    juce::dontSendNotification);
        else if (i == te::IDs::solo)
            soloButton.setToggleState(
                    (bool) v[i],
                    juce::dontSendNotification);
    }
    else if (v.hasType(te::IDs::INPUTDEVICES) ||
             v.hasType(te::IDs::INPUTDEVICE) ||
             v.hasType(te::IDs::INPUTDEVICEDESTINATION))
    {
        if (auto at = dynamic_cast<te::AudioTrack*>(track.get()))
        {
            armButton.setEnabled(
                    ext::track::trackHasInput(*at));
            armButton.setToggleState(
                    ext::track::isTrackArmed(*at),
                    juce::dontSendNotification);
        }
    }
}

void TrackHeaderComponent::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::grey);
    g.fillRect(getLocalBounds().withTrimmedRight(2));

    if (editViewState.selectionManager.isSelected(track.get()))
    {
        g.setColour(juce::Colours::red);
        g.drawRect(
                getLocalBounds().withTrimmedRight(-4),
                2);
    }
}

void TrackHeaderComponent::mouseDown(const juce::MouseEvent&)
{
    editViewState.selectionManager.selectOnly(track.get());
}

void TrackHeaderComponent::resized()
{
    auto r = getLocalBounds().reduced(4);
    trackName.setBounds(
            r.removeFromTop(
                    r.getHeight() / 3));

    int w = r.getHeight();
    inputButton.setBounds(r.removeFromLeft(w));
    r.removeFromLeft(2);
    armButton.setBounds(r.removeFromLeft(w));
    r.removeFromLeft(2);
    addClipButton.setBounds(r.removeFromLeft(w));
    r.removeFromLeft(2);
    muteButton.setBounds(r.removeFromLeft(w));
    r.removeFromLeft(2);
    soloButton.setBounds(r.removeFromLeft(w));
    r.removeFromLeft(2);
}


TrackFooterComponent::TrackFooterComponent(
        EditViewState& evs,
        te::Track::Ptr t)
    : editViewState(evs),
      track(move(t))
{
    addAndMakeVisible(addButton);

    buildPlugins();

    track->state.addListener(this);

    addButton.onClick = [this] {
        if (auto plugin = showMenuAndCreatePlugin(track->edit))
            track->pluginList.insertPlugin(
                    plugin,
                    0,
                    &editViewState.selectionManager);
    };
}

TrackFooterComponent::~TrackFooterComponent()
{
    track->state.removeListener(this);
}

void TrackFooterComponent::valueTreeChildAdded(
        juce::ValueTree&,
        juce::ValueTree& c)
{
    if (c.hasType(te::IDs::PLUGIN))
        markAndUpdate(updatePlugins);
}

void TrackFooterComponent::valueTreeChildRemoved(
        juce::ValueTree&,
        juce::ValueTree& c,
        int)
{
    if (c.hasType(te::IDs::PLUGIN))
        markAndUpdate(updatePlugins);
}

void TrackFooterComponent::valueTreeChildOrderChanged(
        juce::ValueTree&,
        int,
        int)
{
    markAndUpdate(updatePlugins);
}

void TrackFooterComponent::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::grey);
    g.fillRect(getLocalBounds().withTrimmedLeft(2));

    if (editViewState.selectionManager.isSelected(track.get()))
    {
        g.setColour(juce::Colours::red);
        g.drawRect(getLocalBounds().withTrimmedLeft(-4), 2);
    }
}

void TrackFooterComponent::mouseDown(const juce::MouseEvent&)
{
    editViewState.selectionManager.selectOnly(track.get());
}

void TrackFooterComponent::resized()
{
    auto      r = getLocalBounds().reduced(4);
    const int cx = 21;

    addButton.setBounds(r.removeFromLeft(cx).withSizeKeepingCentre(cx, cx));
    r.removeFromLeft(6);

    for (auto p : plugins)
    {
        p->setBounds(r.removeFromLeft(cx).withSizeKeepingCentre(cx, cx));
        r.removeFromLeft(2);
    }
}

void TrackFooterComponent::handleAsyncUpdate()
{
    if (compareAndReset(updatePlugins))
        buildPlugins();
}

void TrackFooterComponent::buildPlugins()
{
    plugins.clear();

    for (auto plugin : track->pluginList)
    {
        auto pluginComponent = new PluginComponent(plugin);
        addAndMakeVisible(pluginComponent);
        plugins.add(pluginComponent);

        pluginComponent->onClick = [this, plugin] {
            editViewState.selectionManager.selectOnly(plugin);
        };
    }
    resized();
}


TrackComponent::TrackComponent(
        EditViewState& evs,
        te::Track::Ptr t)
    : editViewState(evs),
      track(move(t))
{
    track->state.addListener(this);
    track->edit.getTransport().addChangeListener(this);

    markAndUpdate(updateClips);
}

TrackComponent::~TrackComponent()
{
    track->state.removeListener(this);
    track->edit.getTransport().removeChangeListener(this);
}

void TrackComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::grey);

    if (editViewState.selectionManager.isSelected(track.get()))
    {
        g.setColour(juce::Colours::red);

        auto rc = getLocalBounds();
        if (editViewState.showHeaders)
            rc = rc.withTrimmedLeft(-4);
        if (editViewState.showFooters)
            rc = rc.withTrimmedRight(-4);

        g.drawRect(rc, 2);
    }
}

void TrackComponent::mouseDown(const juce::MouseEvent&)
{
    editViewState.selectionManager.selectOnly(track.get());
}

void TrackComponent::changeListenerCallback(juce::ChangeBroadcaster*)
{
    markAndUpdate(updateRecordClips);
}

void TrackComponent::valueTreePropertyChanged(
        juce::ValueTree& v, const juce::Identifier& i)
{
    if (te::Clip::isClipState(v))
    {
        if (i == te::IDs::start || i == te::IDs::length)
        {
            markAndUpdate(updatePositions);
        }
    }
}

void TrackComponent::valueTreeChildAdded(
        juce::ValueTree&, juce::ValueTree& c)
{
    if (te::Clip::isClipState(c))
        markAndUpdate(updateClips);
}

void TrackComponent::valueTreeChildRemoved(
        juce::ValueTree&, juce::ValueTree& c, int)
{
    if (te::Clip::isClipState(c))
        markAndUpdate(updateClips);
}

void TrackComponent::valueTreeChildOrderChanged(
        juce::ValueTree& v, int a, int b)
{
    if (te::Clip::isClipState(v.getChild(b)) ||
        te::Clip::isClipState(v.getChild(a)))
        markAndUpdate(updatePositions);
}

void TrackComponent::handleAsyncUpdate()
{
    if (compareAndReset(updateClips))
        buildClips();
    if (compareAndReset(updatePositions))
        resized();
    if (compareAndReset(updateRecordClips))
        buildRecordClips();
}

void TrackComponent::resized()
{
    for (auto cc : clips)
    {
        auto& c = cc->getClip();
        auto  pos = c.getPosition();
        int   x1 = editViewState.timeToX(pos.getStart(), getWidth());
        int   x2 = editViewState.timeToX(pos.getEnd(), getWidth());

        cc->setBounds(x1, 0, x2 - x1, getHeight());
    }
}

void TrackComponent::buildClips()
{
    clips.clear();

    if (auto ct = dynamic_cast<te::ClipTrack*>(track.get()))
    {
        for (auto c : ct->getClips())
        {
            ClipComponent* cc = nullptr;

            if (dynamic_cast<te::WaveAudioClip*>(c))
                cc = new AudioClipComponent(editViewState, c);
            else if (dynamic_cast<te::MidiClip*>(c))
                cc = new MidiClipComponent(editViewState, c);
            else
                cc = new ClipComponent(editViewState, c);

            clips.add(cc);
            addAndMakeVisible(cc);
        }
    }

    resized();
}

void TrackComponent::buildRecordClips()
{
    bool needed = false;

    if (track->edit.getTransport().isRecording())
    {
        for (auto in : track->edit.getAllInputDevices())
        {
            if (in->isRecordingActive() &&
                track.get() == in->getTargetTracks().getFirst())
            {
                needed = true;
                break;
            }
        }
    }

    if (needed)
    {
        recordingClip = std::make_unique<RecordingClipComponent>(
                track,
                editViewState);
        addAndMakeVisible(*recordingClip);
    }
    else
    {
        recordingClip = nullptr;
    }
}

BLOOPER_NAMESPACE_END
