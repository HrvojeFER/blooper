#include <blooper/edit/ClipComponents.hpp>


BLOOPER_NAMESPACE_BEGIN

ClipComponent::ClipComponent(EditViewState& evs, te::Clip::Ptr c)
    : editViewState(evs), clip(std::move(c))
{
}

void ClipComponent::paint(juce::Graphics& g)
{
    g.fillAll(clip->getColour().withAlpha(0.5f));
    g.setColour(juce::Colours::black);
    g.drawRect(getLocalBounds());

    if (editViewState.selectionManager.isSelected(clip.get()))
    {
        g.setColour(juce::Colours::red);
        g.drawRect(getLocalBounds(), 2);
    }
}

void ClipComponent::mouseDown(const juce::MouseEvent&)
{
    editViewState.selectionManager.selectOnly(clip.get());
}


AudioClipComponent::AudioClipComponent(EditViewState& evs, te::Clip::Ptr c)
    : ClipComponent(evs, std::move(c))
{
    updateThumbnail();
}

void AudioClipComponent::paint(juce::Graphics& g)
{
    ClipComponent::paint(g);

    if (editViewState.drawWaveforms && thumbnail != nullptr)
        drawWaveform(
                g,
                *getWaveAudioClip(),
                *thumbnail,
                juce::Colours::black.withAlpha(0.5f),
                0,
                getWidth(),
                0,
                getHeight(),
                0);
}

void AudioClipComponent::drawWaveform(
        juce::Graphics&     g,
        te::AudioClipBase&  c,
        te::SmartThumbnail& thumb,
        juce::Colour        colour,

        int left,
        int right,
        int y,
        int h,
        int xOffset)
{
    auto getTimeRangeForDrawing =
            [this](const int left, const int right) -> te::EditTimeRange {
        if (auto p = getParentComponent())
        {
            double t1 = editViewState.xToTime(left, p->getWidth());
            double t2 = editViewState.xToTime(right, p->getWidth());

            return {t1, t2};
        }

        return {};
    };

    jassert(left <= right);
    const auto gain = c.getGain();
    const auto pan = thumb.getNumChannels() == 1 ? 0.0f : c.getPan();

    const float pv = pan * gain;
    const float gainL = (gain - pv);
    const float gainR = (gain + pv);

    const bool usesTimeStretchedProxy = c.usesTimeStretchedProxy();

    const auto clipPos = c.getPosition();
    auto       offset = clipPos.getOffset();
    auto       speedRatio = c.getSpeedRatio();

    g.setColour(colour);

    if (usesTimeStretchedProxy)
    {
        const juce::Rectangle<int> area(
                left + xOffset,
                y,
                right - left,
                h);

        if (!thumb.isOutOfDate())
        {
            drawChannels(g, thumb, area, false,
                         getTimeRangeForDrawing(left, right),
                         c.isLeftChannelActive(), c.isRightChannelActive(),
                         gainL, gainR);
        }
    }
    else if (c.getLoopLength() == 0)
    {
        auto region = getTimeRangeForDrawing(left, right);

        auto t1 = (region.getStart() + offset) * speedRatio;
        auto t2 = (region.getEnd() + offset) * speedRatio;

        drawChannels(g, thumb,
                     {left + xOffset, y, right - left, h},
                     false, {t1, t2},
                     c.isLeftChannelActive(), c.isRightChannelActive(),
                     gainL, gainR);
    }
}

void AudioClipComponent::drawChannels(
        juce::Graphics&      g,
        te::SmartThumbnail&  thumb,
        juce::Rectangle<int> area,
        bool                 useHighRes,

        te::EditTimeRange time,

        bool useLeft,
        bool useRight,

        float leftGain,
        float rightGain)
{
    if (useLeft && useRight && thumb.getNumChannels() > 1)
    {
        thumb.drawChannel(
                g,
                area.removeFromTop(area.getHeight() / 2),
                useHighRes,
                time,
                0,
                leftGain);

        thumb.drawChannel(
                g,
                area,
                useHighRes,
                time,
                1,
                rightGain);
    }
    else if (useLeft)
    {
        thumb.drawChannel(
                g, area,
                useHighRes,
                time,
                0,
                leftGain);
    }
    else if (useRight)
    {
        thumb.drawChannel(
                g,
                area,
                useHighRes,
                time,
                1,
                rightGain);
    }
}

void AudioClipComponent::updateThumbnail()
{
    if (auto* wac = getWaveAudioClip())
    {
        te::AudioFile af(wac->getAudioFile());

        if (af.getFile().existsAsFile() || (!wac->usesSourceFile()))
        {
            if (af.isValid())
            {
                const te::AudioFile proxy(
                        (wac->hasAnyTakes() &&
                         wac->isShowingTakes()) ?
                                wac->getAudioFile() :
                                wac->getPlaybackFile());

                if (thumbnail == nullptr)
                    thumbnail = std::make_unique<te::SmartThumbnail>(
                            wac->edit.engine,
                            proxy,
                            *this, &wac->edit);
                else
                    thumbnail->setNewFile(proxy);
            }
            else
            {
                thumbnail = nullptr;
            }
        }
    }
}


MidiClipComponent::MidiClipComponent(EditViewState& evs, te::Clip::Ptr c)
    : ClipComponent(evs, std::move(c))
{
}

void MidiClipComponent::paint(juce::Graphics& g)
{
    ClipComponent::paint(g);

    if (auto mc = getMidiClip())
    {
        auto& seq = mc->getSequence();
        for (auto n : seq.getNotes())
        {
            double sBeat = mc->getStartBeat() + n->getStartBeat();
            double eBeat = mc->getStartBeat() + n->getEndBeat();

            auto s = editViewState.beatToTime(sBeat);
            auto e = editViewState.beatToTime(eBeat);

            if (auto p = getParentComponent())
            {
                double t1 = editViewState.timeToX(s, p->getWidth()) - getX();
                double t2 = editViewState.timeToX(e, p->getWidth()) - getX();

                double y =
                        (1.0 - double(n->getNoteNumber()) / 127.0) *
                        getHeight();

                g.setColour(juce::Colours::white.withAlpha(
                        static_cast<float>(n->getVelocity()) /
                        127.0f));

                g.drawLine(float(t1),
                           float(y),
                           float(t2),
                           float(y));
            }
        }
    }
}


RecordingClipComponent::RecordingClipComponent(
        te::Track::Ptr t, EditViewState& evs)
    : track(std::move(t)), editViewState(evs)
{
    startTimerHz(10);
    initialiseThumbnailAndPunchTime();
}

void RecordingClipComponent::initialiseThumbnailAndPunchTime()
{
    if (auto at = dynamic_cast<te::AudioTrack*>(track.get()))
    {
        for (auto* idi : at->edit.getEditInputDevices()
                                 .getDevicesForTargetTrack(*at))
        {
            punchInTime = idi->getPunchInTime();

            if (idi->getRecordingFile().exists())
                thumbnail =
                        at->edit.engine
                                .getRecordingThumbnailManager()
                                .getThumbnailFor(idi->getRecordingFile());
        }
    }
}

void RecordingClipComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::red.withAlpha(0.5f));
    g.setColour(juce::Colours::black);
    g.drawRect(getLocalBounds());

    if (editViewState.drawWaveforms)
        drawThumbnail(
                g,
                juce::Colours::black.withAlpha(0.5f));
}

void RecordingClipComponent::drawThumbnail(
        juce::Graphics& g, juce::Colour waveformColour) const
{
    if (thumbnail == nullptr)
        return;

    juce::Rectangle<int> bounds;
    juce::Range<double>  times;
    getBoundsAndTime(bounds, times);
    auto w = bounds.getWidth();

    if (w > 0 && w < 10000)
    {
        g.setColour(waveformColour);
        thumbnail->thumb.drawChannels(
                g,
                bounds,
                w,
                times,
                1.0f);
    }
}

bool RecordingClipComponent::getBoundsAndTime(
        juce::Rectangle<int>& bounds, juce::Range<double>& times) const
{
    auto editTimeToX = [this](double t) {
        if (auto p = getParentComponent())
            return editViewState.timeToX(t, p->getWidth()) - getX();
        return 0;
    };

    auto xToEditTime = [this](int x) {
        if (auto p = getParentComponent())
            return editViewState.xToTime(x + getX(), p->getWidth());
        return 0.0;
    };

    bool  hasLooped = false;
    auto& edit = track->edit;

    if (auto* playhead = edit.getTransport().getCurrentPlayhead())
    {
        auto localBounds = getLocalBounds();

        auto timeStarted = thumbnail->punchInTime;
        auto unloopedPos = timeStarted + thumbnail->thumb.getTotalLength();

        auto t1 = timeStarted;
        auto t2 = unloopedPos;

        if (playhead->isLooping() && t2 >= playhead->getLoopTimes().end)
        {
            hasLooped = true;

            t1 = juce::jmin(t1, playhead->getLoopTimes().start);
            t2 = playhead->getPosition();

            t1 = juce::jmax(editViewState.viewX1.get(), t1);
            t2 = juce::jmin(editViewState.viewX2.get(), t2);
        }
        else if (edit.recordingPunchInOut)
        {
            const double in = thumbnail->punchInTime;
            const double out = edit.getTransport().getLoopRange().getEnd();

            t1 = juce::jlimit(in, out, t1);
            t2 = juce::jlimit(in, out, t2);
        }

        bounds = localBounds
                         .withX(juce::jmax(
                                 localBounds.getX(),
                                 editTimeToX(t1)))
                         .withRight(juce::jmin(
                                 localBounds.getRight(),
                                 editTimeToX(t2)));

        auto         loopRange = playhead->getLoopTimes();
        const double recordedTime = unloopedPos - playhead->getLoopTimes().start;
        const int    numLoops = (int) (recordedTime / loopRange.getLength());

        const juce::Range<double> editTimes(xToEditTime(bounds.getX()),
                                            xToEditTime(bounds.getRight()));

        times = (editTimes + (numLoops * loopRange.getLength())) - timeStarted;
    }

    return hasLooped;
}

void RecordingClipComponent::timerCallback()
{
    updatePosition();
}

void RecordingClipComponent::updatePosition()
{
    auto& edit = track->edit;

    if (auto playhead = edit.getTransport().getCurrentPlayhead())
    {
        double t1 =
                punchInTime >= 0 ?
                        punchInTime :
                        edit.getTransport().getTimeWhenStarted();
        double t2 = juce::jmax(t1, playhead->getUnloopedPosition());

        if (playhead->isLooping())
        {
            auto loopTimes = playhead->getLoopTimes();

            if (t2 >= loopTimes.end)
            {
                t1 = juce::jmin(t1, loopTimes.start);
                t2 = loopTimes.end;
            }
        }
        else if (edit.recordingPunchInOut)
        {
            auto mr = edit.getTransport().getLoopRange();
            auto in = mr.getStart();
            auto out = mr.getEnd();

            t1 = juce::jlimit(in, out, t1);
            t2 = juce::jlimit(in, out, t2);
        }

        t1 = juce::jmax(t1, editViewState.viewX1.get());
        t2 = juce::jmin(t2, editViewState.viewX2.get());

        if (auto p = getParentComponent())
        {
            int x1 = editViewState.timeToX(t1, p->getWidth());
            int x2 = editViewState.timeToX(t2, p->getWidth());

            setBounds(x1, 0, x2 - x1, p->getHeight());
            return;
        }
    }

    setBounds({});
}

BLOOPER_NAMESPACE_END
