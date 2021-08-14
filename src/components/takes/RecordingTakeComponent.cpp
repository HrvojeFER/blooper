#include <blooper/components/takes/RecordingTakeComponent.hpp>

#include <blooper/internal/utils/gui.hpp>

BLOOPER_NAMESPACE_BEGIN

RecordingTakeComponent::RecordingTakeComponent(
    AbstractContext&              context,
    State                         state,
    WaveAudioTakeRef              take,
    RecordingTakeComponentOptions options)
    : RecordingTakeComponent::base(
          context,
          move(state),
          move(take)),
      options(move(options))
{
  this->updateThumbnail();
  this->updatePosition();

  if (this->options.shouldResizeItself)
    this->startTimerHz(defaultGuiTimerHz);
}

RecordingTakeComponent::~RecordingTakeComponent()
{
  this->stopTimer();
}


void RecordingTakeComponent::drawWaveform(
    JuceGraphics&  g,
    JuceThumbnail& thumb,
    JuceTimeRange  time,

    JuceBounds bounds,
    JuceColour waveformColour)
{
  g.setColour(waveformColour);
  thumb.drawChannels(
      g,
      move(bounds),

      true,
      move(time),
      1.0f);
}


void RecordingTakeComponent::updateThumbnail()
{
  auto& clip = this->getClip();

  auto audioTrackPtr = dynamic_cast<te::AudioTrack*>(clip.getTrack());
  if (!audioTrackPtr) return;
  auto& audioTrack = *audioTrackPtr;

  auto& engine = this->getContext().getEngine();
  auto& edit = clip.edit;

  for (auto* device :
       audioTrack
           .edit
           .getEditInputDevices()
           .getDevicesForTargetTrack(audioTrack))
  {
    if (device->getRecordingFile().exists())
    {
      this->thumbnail =
          engine
              .getRecordingThumbnailManager()
              .getThumbnailFor(device->getRecordingFile());
    }
  }
}

void RecordingTakeComponent::updatePosition()
{
  this->setBounds(
      util::withHorizontalRange(
          this->getBounds(),
          this->getTimePixelMapping().pixels));

  if (this->options.shouldResizeParentWhenResized &&
      this->getParentComponent())
    this->getParentComponent()->resized();
}


// AbstractTimePixelConverter

JuceTimeRange RecordingTakeComponent::getAbsoluteTimeRange() const noexcept
{
  auto& thumb = *this->thumbnail;

  auto& edit = this->getClip().edit;
  auto& transport = edit.getTransport();

  TimePixelMapping result{};

  const auto timeStarted = thumb.punchInTime;
  const auto unloopedPosition = timeStarted + thumb.thumb.getTotalLength();
  const auto loopRange = transport.getLoopRange();

  auto start = timeStarted;
  auto end = unloopedPosition;

  if (transport.looping && end >= loopRange.end)
  {
    start = std::min(start, loopRange.start);
    end = transport.position;
  }
  else if (edit.recordingPunchInOut)
  {
    const double in = thumbnail->punchInTime;
    const double out = edit.getTransport().getLoopRange().getEnd();

    start = std::clamp(start, in, out);
    end = std::clamp(end, in, out);
  }

  return {start, end};
}


// Component

void RecordingTakeComponent::paint(juce::Graphics& g)
{
  g.fillAll(this->findColour(ColourId::background)
                .withAlpha(0.5f));

  g.setColour(this->findColour(ColourId::black));
  g.drawRect(this->getLocalBounds());

  const auto mapping = this->getTimePixelMapping();

  RecordingTakeComponent::drawWaveform(
      g,
      this->thumbnail->thumb,
      mapping.time,

      util::withHorizontalRange(this->getLocalBounds(), mapping.pixels),
      this->findColour(ColourId::red)
          .withAlpha(0.5f));
}


// Timer

void RecordingTakeComponent::timerCallback()
{
  this->updatePosition();
}

BLOOPER_NAMESPACE_END
