#ifndef BLOOPER_UTILITIES_HPP
#define BLOOPER_UTILITIES_HPP


namespace blooper
{
using namespace juce;
namespace te = tracktion_engine;


namespace Helpers
{
static inline void addAndMakeVisible(
        Component&               parent,
        const Array<Component*>& children)
{
    for (auto c : children)
        parent.addAndMakeVisible(c);
}

[[maybe_unused]] static inline String getStringOrDefault(
        const String& stringToTest,
        const String& stringToReturnIfEmpty)
{
    return stringToTest.isEmpty() ?
                   stringToReturnIfEmpty :
                   stringToTest;
}

static inline File findRecentEdit(const File& dir)
{
    auto files = dir.findChildFiles(
            File::findFiles,
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


namespace PlayHeadHelpers
{
static inline String timeToTimeCodeString(double seconds)
{
    const auto milliseconds = roundToInt(seconds * 1000.0);
    const auto absMilliseconds = std::abs(milliseconds);

    return String::formatted(
            "%02d:%02d:%02d.%03d",
            milliseconds / 3600000,
            (absMilliseconds / 60000) % 60,
            (absMilliseconds / 1000) % 60,
            absMilliseconds % 1000);
}

static inline String quarterNotePositionToBarsBeatsString(
        double quarterNotes,
        int    numerator,
        int    denominator)
{
    if (numerator == 0 || denominator == 0)
        return "1|1|000";

    auto quarterNotesPerBar = ((double) numerator * 4.0 /
                               (double) denominator);
    auto beats = (fmod(quarterNotes, quarterNotesPerBar) /
                  quarterNotesPerBar) *
                 numerator;

    auto bar = ((int) quarterNotes) / quarterNotesPerBar + 1;
    auto beat = ((int) beats) + 1;
    auto ticks = (lround((fmod(beats, 1.0) * 960.0 + 0.5)));

    return String::formatted("%d|%d|%03d", bar, beat, ticks);
}

[[maybe_unused]] static inline String getTimeCodeDisplay(
        const AudioPlayHead::CurrentPositionInfo& pos)
{
    MemoryOutputStream displayText;

    const auto barsBeats =
            quarterNotePositionToBarsBeatsString(
                    pos.ppqPosition,
                    pos.timeSigNumerator,
                    pos.timeSigDenominator);

    displayText << String(pos.bpm, 2)
                << " bpm, "
                << pos.timeSigNumerator
                << '/'
                << pos.timeSigDenominator
                << "  -  "
                << timeToTimeCodeString(pos.timeInSeconds)
                << "  -  "
                << barsBeats;

    if (pos.isRecording)
        displayText << "  (recording)";
    else if (pos.isPlaying)
        displayText << "  (playing)";
    else
        displayText << "  (stopped)";

    return displayText.toString();
}
} // namespace PlayHeadHelpers


namespace EngineHelpers
{
[[maybe_unused]] static inline te::Project::Ptr createTempProject(
        te::Engine& engine)
{
    auto file = engine.getTemporaryFileManager()
                        .getTempDirectory()
                        .getChildFile("temp_project")
                        .withFileExtension(te::projectFileSuffix);

    te::ProjectManager::TempProject tempProject(
            engine.getProjectManager(),
            file,
            true);

    return tempProject.project;
}

[[maybe_unused]] static inline void showAudioDeviceSettings(
        te::Engine& engine)
{
    DialogWindow::LaunchOptions o;
    o.dialogTitle = TRANS("Audio Settings");
    o.dialogBackgroundColour =
            LookAndFeel::getDefaultLookAndFeel()
                    .findColour(ResizableWindow::backgroundColourId);

    o.content.setOwned(
            new AudioDeviceSelectorComponent(
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

[[maybe_unused]] static inline void showPluginSettings(
        te::Engine& engine)
{
    DialogWindow::LaunchOptions o;
    o.dialogTitle = TRANS("Plugins");
    o.dialogBackgroundColour = Colours::black;
    o.escapeKeyTriggersCloseButton = true;
    o.useNativeTitleBar = true;
    o.resizable = true;
    o.useBottomRightCornerResizer = true;

    auto v = new PluginListComponent(
            engine.getPluginManager().pluginFormatManager,
            engine.getPluginManager().knownPluginList,
            engine.getTemporaryFileManager()
                    .getTempFile("PluginScanDeadMansPedal"),
            te::getApplicationSettings());

    v->setSize(800, 600);
    o.content.setOwned(v);
    o.launchAsync();
}

[[maybe_unused]] static inline void browseForAudioFile(
        te::Engine&                      engine,
        std::function<void(const File&)> fileChosenCallback)
{
    auto fc = std::make_shared<FileChooser>(
            "Please select an audio file to load...",
            engine.getPropertyStorage()
                    .getDefaultLoadSaveDirectory("pitchAndTimeExample"),
            engine.getAudioFileFormatManager()
                    .readFormatManager
                    .getWildcardForAllFormats());

    fc->launchAsync(
            FileBrowserComponent::openMode +
                    FileBrowserComponent::canSelectFiles,
            [fc, &engine, callback = std::move(fileChosenCallback)](
                    const FileChooser&) {
                const auto f = fc->getResult();

                if (f.existsAsFile())
                    engine.getPropertyStorage()
                            .setDefaultLoadSaveDirectory(
                                    "pitchAndTimeExample",
                                    f.getParentDirectory());

                callback(f);
            });
}

static inline void removeAllClips(te::AudioTrack& track)
{
    const auto& clips = track.getClips();

    for (int i = clips.size(); --i >= 0;)
        clips.getUnchecked(i)->removeFromParentTrack();
}

static inline te::AudioTrack* getOrInsertAudioTrackAt(
        te::Edit& edit, int index)
{
    edit.ensureNumberOfAudioTracks(index + 1);
    return te::getAudioTracks(edit)[index];
}

[[maybe_unused]] static inline te::WaveAudioClip::Ptr loadAudioFileAsClip(
        te::Edit&   edit,
        const File& file)
{
    if (auto track = getOrInsertAudioTrackAt(edit, 0))
    {
        removeAllClips(*track);

        te::AudioFile audioFile(edit.engine, file);

        if (audioFile.isValid())
            if (auto newClip =
                        track->insertWaveClip(
                                file.getFileNameWithoutExtension(),
                                file,
                                {{0.0,
                                  audioFile.getLength()},
                                 0.0},
                                false))
                return newClip;
    }

    return {};
}

template<typename ClipType>
[[maybe_unused]] static inline typename ClipType::Ptr loopAroundClip(
        ClipType& clip)
{
    auto& transport = clip.edit.getTransport();
    transport.setLoopRange(clip.getEditTimeRange());
    transport.looping = true;
    transport.position = 0.0;
    transport.play(false);

    return clip;
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

    if (auto tree = KnownPluginList::createTree(
                list.getTypes(),
                KnownPluginList::sortByManufacturer))
        return tree;

    return {};
}
} // namespace EngineHelpers


namespace
{
class FlaggedAsyncUpdater : public AsyncUpdater
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


struct Thumbnail : public Component
{
    explicit Thumbnail(te::TransportControl& tc)
        : transport(tc)
    {
        cursorUpdater.setCallback([this] {
            updateCursorPosition();

            if (smartThumbnail.isGeneratingProxy() ||
                smartThumbnail.isOutOfDate())
                repaint();
        });

        cursor.setFill(findColour(Label::textColourId));
        addAndMakeVisible(cursor);
    }

    void setFile(const te::AudioFile& file)
    {
        smartThumbnail.setNewFile(file);
        cursorUpdater.startTimerHz(25);
        repaint();
    }

    void paint(Graphics& g) override
    {
        auto       r = getLocalBounds();
        const auto colour = findColour(Label::textColourId);

        if (smartThumbnail.isGeneratingProxy())
        {
            g.setColour(
                    colour.withMultipliedBrightness(0.9f));
            g.drawText(
                    "Creating proxy: " +
                            String(roundToInt(
                                    smartThumbnail.getProxyProgress() *
                                    100.0f)) +
                            "%",
                    r, Justification::centred);
        }
        else
        {
            const float brightness =
                    smartThumbnail.isOutOfDate() ?
                            0.4f :
                            0.66f;
            g.setColour(colour.withMultipliedBrightness(brightness));
            smartThumbnail.drawChannels(
                    g, r,
                    true,
                    {0.0, smartThumbnail.getTotalLength()},
                    1.0f);
        }
    }

    void mouseDown(const MouseEvent& e) override
    {
        transport.setUserDragging(true);
        mouseDrag(e);
    }

    void mouseDrag(const MouseEvent& e) override
    {
        jassert(getWidth() > 0);

        const float proportion =
                e.position.x /
                static_cast<float>(getWidth());

        transport.position =
                proportion *
                transport.getLoopRange().getLength();
    }

    void mouseUp(const MouseEvent&) override
    {
        transport.setUserDragging(false);
    }


private:
    [[maybe_unused]] te::TransportControl& transport;

    te::SmartThumbnail smartThumbnail{
            transport.engine,
            te::AudioFile(transport.engine),
            *this,
            nullptr};

    [[maybe_unused]] DrawableRectangle cursor;
    [[maybe_unused]] te::LambdaTimer   cursorUpdater;


    void updateCursorPosition()
    {
        const double loopLength = transport.getLoopRange().getLength();
        const double proportion =
                loopLength == 0.0 ?
                        0.0 :
                        transport.getCurrentPosition() / loopLength;

        auto        r = getLocalBounds().toFloat();
        const float x = r.getWidth() * float(proportion);
        cursor.setRectangle(r.withWidth(2.0f).withX(x));
    }
};
} // namespace
} // namespace blooper


#endif // BLOOPER_UTILITIES_HPP
