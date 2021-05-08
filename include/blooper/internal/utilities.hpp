#ifndef BLOOPER_UTILITIES_HPP
#define BLOOPER_UTILITIES_HPP


BLOOPER_NAMESPACE_BEGIN

namespace Helpers
{
static inline void addAndMakeVisible(
        juce::Component&                     parent,
        const juce::Array<juce::Component*>& children)
{
    for (auto c : children)
        parent.addAndMakeVisible(c);
}

static inline juce::File findRecentEdit(const juce::File& dir)
{
    auto files = dir.findChildFiles(
            juce::File::findFiles,
            false,
            "*.tracktionedit");

    if (files.size() > 0)
    {
        files.sort();
        return files.getLast();
    }

    return {};
}
} // namespace Helpers


namespace EngineHelpers
{
static inline void showAudioDeviceSettings(te::Engine& engine)
{
    juce::DialogWindow::LaunchOptions o;
    o.dialogTitle = TRANS("Audio Settings");
    o.dialogBackgroundColour =
            juce::LookAndFeel::getDefaultLookAndFeel()
                    .findColour(juce::ResizableWindow::backgroundColourId);

    o.content.setOwned(
            new juce::AudioDeviceSelectorComponent(
                    engine.getDeviceManager().deviceManager,
                    0,
                    512,
                    1,
                    512,
                    false,
                    false,
                    true,
                    true));

    o.content->setSize(400, 600);
    o.launchAsync();
}

static inline void showPluginSettings(te::Engine& engine)
{
    juce::DialogWindow::LaunchOptions o;
    o.dialogTitle = TRANS("Plugins");
    o.dialogBackgroundColour = juce::Colours::black;
    o.escapeKeyTriggersCloseButton = true;
    o.useNativeTitleBar = true;
    o.resizable = true;
    o.useBottomRightCornerResizer = true;

    auto v = new juce::PluginListComponent(
            engine.getPluginManager().pluginFormatManager,
            engine.getPluginManager().knownPluginList,
            engine.getTemporaryFileManager()
                    .getTempFile("PluginScanDeadMansPedal"),
            te::getApplicationSettings());

    v->setSize(800, 600);
    o.content.setOwned(v);
    o.launchAsync();
}

static inline te::AudioTrack* getOrInsertAudioTrackAt(
        te::Edit& edit, int index)
{
    edit.ensureNumberOfAudioTracks(index + 1);
    return te::getAudioTracks(edit)[index];
}

static inline void togglePlay(te::Edit& edit)
{
    auto& transport = edit.getTransport();

    if (transport.isPlaying())
        transport.stop(false, false);
    else
        transport.play(false);
}

static inline void toggleRecord(te::Edit& edit)
{
    auto& transport = edit.getTransport();

    if (transport.isRecording())
        transport.stop(true, false);
    else
        transport.record(false);
}

static inline void armTrack(
        te::AudioTrack& t,
        bool            arm,
        int             position = 0)
{
    auto& edit = t.edit;
    for (auto instance : edit.getAllInputDevices())
        if (instance->isOnTargetTrack(t, position))
            instance->setRecordingEnabled(t, arm);
}

static inline bool isTrackArmed(
        te::AudioTrack& t,
        int             position = 0)
{
    auto& edit = t.edit;
    for (auto instance : edit.getAllInputDevices())
        if (instance->isOnTargetTrack(t, position))
            return instance->isRecordingEnabled(t);

    return false;
}

static inline bool isInputMonitoringEnabled(
        te::AudioTrack& t,
        int             position = 0)
{
    auto& edit = t.edit;
    for (auto instance : edit.getAllInputDevices())
        if (instance->isOnTargetTrack(t, position))
            return instance->getInputDevice().isEndToEndEnabled();

    return false;
}

static inline void enableInputMonitoring(
        te::AudioTrack& t,
        bool            im,
        int             position = 0)
{
    if (isInputMonitoringEnabled(t, position) != im)
    {
        auto& edit = t.edit;
        for (auto instance : edit.getAllInputDevices())
            if (instance->isOnTargetTrack(t, position))
                instance->getInputDevice().flipEndToEnd();
    }
}

static inline bool trackHasInput(te::AudioTrack& t, int position = 0)
{
    auto& edit = t.edit;
    for (auto instance : edit.getAllInputDevices())
        if (instance->isOnTargetTrack(t, position))
            return true;

    return false;
}

static inline std::unique_ptr<juce::KnownPluginList::PluginTree>
createPluginTree(te::Engine& engine)
{
    auto& list = engine.getPluginManager().knownPluginList;

    if (auto tree = juce::KnownPluginList::createTree(
                list.getTypes(),
                juce::KnownPluginList::sortByManufacturer))
        return tree;

    return {};
}
} // namespace EngineHelpers


namespace
{
class FlaggedAsyncUpdater : public juce::AsyncUpdater
{
public:
    void markAndUpdate(bool& flag)
    {
        flag = true;
        triggerAsyncUpdate();
    }

    static bool compareAndReset(bool& flag) noexcept
    {
        if (!flag)
            return false;

        flag = false;
        return true;
    }
};
} // namespace

BLOOPER_NAMESPACE_END


#endif // BLOOPER_UTILITIES_HPP
