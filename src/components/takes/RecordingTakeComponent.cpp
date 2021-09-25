#include <blooper/components/takes/RecordingTakeComponent.hpp>

#include <blooper/internal/utils/gui.hpp>

#include <blooper/internal/ext/thumbnail.hpp>

BLOOPER_NAMESPACE_BEGIN

RecordingTakeComponent::RecordingTakeComponent(
    AbstractContext&              context,
    State                         state,
    AudioTrackRef                 track,
    RecordingTakeComponentOptions options)
    : RecordingTakeComponent::base(
          context,
          move(state),
          move(track)),
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
  this->thumbnail =
      ext::getRecordingThumbnail(
          this->getHeldTrack());
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
  if (!this->thumbnail) return {0.0, 0.0};

  return ext::getTimeRange(
      *this->thumbnail,
      this->getTrack().edit);
}


// Component

void RecordingTakeComponent::paint(juce::Graphics& g)
{
  g.fillAll(this->findColour(ColourId::background)
                .withAlpha(0.5f));

  g.setColour(this->findColour(ColourId::black));
  g.drawRect(this->getLocalBounds());

  RecordingTakeComponent::drawWaveform(
      g,
      this->thumbnail->thumb,
      this->getTimePixelMapping().time,

      util::withHorizontalRange(
          this->getLocalBounds(),
          this->getTimePixelMapping().pixels),
      this->findColour(ColourId::red)
          .withAlpha(0.5f));
}


// Timer

void RecordingTakeComponent::timerCallback()
{
  this->updatePosition();
}

BLOOPER_NAMESPACE_END
