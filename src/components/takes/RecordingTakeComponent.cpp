#include <blooper/components/takes/RecordingTakeComponent.hpp>

#include <blooper/internal/utils/style.hpp>

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
      options(move(options)),

      punchInTime(0)
{
  this->updateThumbnailAndPunchTime();
  this->updatePosition();

  this->startTimerHz(defaultGuiTimerHz);
}

RecordingTakeComponent::~RecordingTakeComponent()
{
  this->stopTimer();
}


// TODO: range

RecordingTakeComponent::BoundsAndTime
RecordingTakeComponent::getBoundsAndTime() const
{
  if (!this->options.converter) return {false};
  auto& converter = *this->options.converter;

  if (this->getTakeRef().isInvalid()) return {false};
  auto& edit = this->getTakeRef().clip->edit;

  if (!edit.getTransport().getCurrentPlayhead()) return {false};
  auto& playhead = *edit.getTransport().getCurrentPlayhead();

  if (!this->thumbnail) return {false};
  auto& thumb = *this->thumbnail;


  BoundsAndTime result{};

  auto availableArea = getLocalBounds();

  const auto timeStarted = thumb.punchInTime;
  const auto unloopedPosition = timeStarted + thumb.thumb.getTotalLength();

  const auto loopRange = playhead.getLoopTimes();
  const auto recordedTime = unloopedPosition - loopRange.start;
  const auto numLoops = static_cast<int>(recordedTime / loopRange.getLength());

  auto start = timeStarted;
  auto end = unloopedPosition;

  if (playhead.isLooping() && end >= loopRange.end)
  {
    result.hasLooped = true;

    start = std::min(start, loopRange.start);
    end = playhead.getPosition();
  }
  else if (edit.recordingPunchInOut)
  {
    const double in = thumbnail->punchInTime;
    const double out = edit.getTransport().getLoopRange().getEnd();

    start = std::clamp(start, in, out);
    end = std::clamp(end, in, out);
  }


  auto timeToX = [this, &converter](double time) -> int {
    if (!this->getParentComponent()) return 0;
    auto& parent = *this->getParentComponent();

    return static_cast<int>(
               converter.convertToProgress(time)) *
               parent.getWidth() -
           this->getX();
  };

  auto xToTime = [this, &converter](int x) {
    if (!this->getParentComponent()) return 0.0;
    auto& parent = *this->getParentComponent();

    return converter.convertToTime(
        static_cast<double>(x + this->getX()) /
        parent.getWidth());
  };

  result.bounds =
      availableArea
          .withX(std::max(availableArea.getX(), timeToX(start)))
          .withRight(std::min(availableArea.getRight(), timeToX(end)));

  const juce::Range<double> editTimes(
      xToTime(result.bounds.getX()),
      xToTime(result.bounds.getRight()));

  result.time =
      (editTimes + numLoops * loopRange.getLength()) - timeStarted;

  return move(result);
}


void RecordingTakeComponent::drawThumbnail(
    juce::Graphics& g,
    juce::Colour    waveformColour) const
{
  if (this->thumbnail == nullptr) return;
  auto& thumb = this->thumbnail->thumb;

  auto boundsAndTime = this->getBoundsAndTime();
  if (!boundsAndTime.isValid) return;

  g.setColour(waveformColour);
  thumb.drawChannels(
      g,
      boundsAndTime.bounds,

      true,
      boundsAndTime.time,
      1.0f);
}


void RecordingTakeComponent::updateThumbnailAndPunchTime()
{
  auto clipPtr = this->getTakeRef().clip;
  if (!clipPtr) return;
  auto& clip = *clipPtr;

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
      this->punchInTime = device->getPunchInTime();

      this->thumbnail =
          engine
              .getRecordingThumbnailManager()
              .getThumbnailFor(device->getRecordingFile());
    }
  }
}

void RecordingTakeComponent::updatePosition()
{
  if (!this->options.shouldResizeItself) return;

  auto boundsAndTime = this->getBoundsAndTime();
  if (!boundsAndTime.isValid) return;

  if (this->options.shouldResizeItself)
  {
    this->setBounds(boundsAndTime.bounds);

    if (this->options.shouldResizeParentWhenResized &&
        this->getParentComponent())
      this->getParentComponent()->resized();
  }
}


// Component

void RecordingTakeComponent::resized()
{
}

void RecordingTakeComponent::paint(juce::Graphics& g)
{
  g.fillAll(
      this->findColour(ColourId::background)
          .withAlpha(0.5f));

  g.setColour(this->findColour(ColourId::black));
  g.drawRect(this->getLocalBounds());

  this->drawThumbnail(
      g,
      this->findColour(ColourId::red)
          .withAlpha(0.5f));
}


// Timer

void RecordingTakeComponent::timerCallback()
{
  this->updatePosition();
}

BLOOPER_NAMESPACE_END
