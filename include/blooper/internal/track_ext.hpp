#ifndef BLOOPER_TRACK_EXT_HPP
#define BLOOPER_TRACK_EXT_HPP


BLOOPER_NAMESPACE_BEGIN

namespace ext::track
{
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

static inline void showClipLoader(te::AudioTrack& t)
{
    class ClipBrowserListener : public juce::FileBrowserListener
    {
    public:
        ClipBrowserListener(te::AudioTrack& track) : track{track} { }
        ~ClipBrowserListener() override = default;


        void selectionChanged() override { }
        void fileClicked(
                const juce::File&,
                const juce::MouseEvent&) override { }

        void browserRootChanged(const juce::File&) override { }


        void fileDoubleClicked(const juce::File& file) override
        {
            track.insertWaveClip(
                    file.getFileName(),
                    file,
                    te::ClipPosition{
                            te::EditTimeRange{},
                            0},
                    false);
        }

    private:
        te::AudioTrack& track;
    };
    auto listener = new ClipBrowserListener(t);

    auto clipBrowser = new juce::FileBrowserComponent(
            juce::FileBrowserComponent::canSelectFiles |
                    juce::FileBrowserComponent::openMode,
            {},
            nullptr,
            nullptr);

    clipBrowser->addListener(listener);
    clipBrowser->setSize(800, 600);

    juce::DialogWindow::LaunchOptions launch;
    launch.dialogTitle = TRANS("Add clip...");
    launch.dialogBackgroundColour = juce::Colours::black;
    launch.escapeKeyTriggersCloseButton = true;
    launch.useNativeTitleBar = true;
    launch.resizable = true;
    launch.useBottomRightCornerResizer = true;

    launch.content.setOwned(clipBrowser);
    launch.launchAsync();
}

[[maybe_unused]] static inline void loopClips(
        const te::ClipTrack& track,
        te::EditTimeRange    range)
{
    for (auto clip : track.getClips())
        clip->setLoopRange(range);
}

[[maybe_unused]] static inline void extendClips(
        const te::ClipTrack& track,
        double               to)
{
    for (auto clip : track.getClips())
        clip->setLength(to, true);
}
} // namespace ext::track

BLOOPER_NAMESPACE_END


#endif // BLOOPER_TRACK_EXT_HPP
