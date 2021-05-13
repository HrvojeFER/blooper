#ifndef BLOOPER_CLIP_COMPONENTS_HPP
#define BLOOPER_CLIP_COMPONENTS_HPP


#include <blooper/internal/internal.hpp>

#include <blooper/body/panels/edit/EditViewState.hpp>


BLOOPER_NAMESPACE_BEGIN

class ClipComponent : public juce::Component
{
public:
    ClipComponent(EditViewState&, te::Clip::Ptr);


    void paint(juce::Graphics& g) override;
    void mouseDown(const juce::MouseEvent& e) override;


    te::Clip& getClip()
    {
        return *clip;
    }


protected:
    EditViewState& editViewState;
    te::Clip::Ptr  clip;
};


class AudioClipComponent : public ClipComponent
{
public:
    AudioClipComponent(EditViewState&, te::Clip::Ptr);


    void paint(juce::Graphics& g) override;


    te::WaveAudioClip* getWaveAudioClip()
    {
        return dynamic_cast<te::WaveAudioClip*>(clip.get());
    }


private:
    void updateThumbnail();


    void drawWaveform(
            juce::Graphics&     g,
            te::AudioClipBase&  c,
            te::SmartThumbnail& thumb,
            juce::Colour        colour,

            int left,
            int right,
            int y,
            int h,
            int xOffset);

    static void drawChannels(
            juce::Graphics&      g,
            te::SmartThumbnail&  thumb,
            juce::Rectangle<int> area,
            bool                 useHighRes,

            te::EditTimeRange time,

            bool useLeft,
            bool useRight,

            float leftGain,
            float rightGain);


    std::unique_ptr<te::SmartThumbnail> thumbnail;
};

class MidiClipComponent : public ClipComponent
{
public:
    MidiClipComponent(EditViewState&, te::Clip::Ptr);

    te::MidiClip* getMidiClip()
    {
        return dynamic_cast<te::MidiClip*>(clip.get());
    }

    void paint(juce::Graphics& g) override;
};


class RecordingClipComponent : public juce::Component, private juce::Timer
{
public:
    RecordingClipComponent(te::Track::Ptr t, EditViewState&);

    void paint(juce::Graphics& g) override;


private:
    void timerCallback() override;

    void updatePosition();

    void initialiseThumbnailAndPunchTime();

    void drawThumbnail(
            juce::Graphics& g,
            juce::Colour    waveformColour) const;

    bool getBoundsAndTime(
            juce::Rectangle<int>& bounds,
            juce::Range<double>&  times) const;


    te::Track::Ptr track;
    EditViewState& editViewState;

    te::RecordingThumbnailManager::Thumbnail::Ptr thumbnail;

    double punchInTime = -1.0;
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_CLIP_COMPONENTS_HPP
