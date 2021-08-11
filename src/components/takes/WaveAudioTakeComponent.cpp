#include <blooper/components/takes/WaveAudioTakeComponent.hpp>

#include <blooper/internal/utils/style.hpp>

BLOOPER_NAMESPACE_BEGIN

WaveAudioTakeComponent::WaveAudioTakeComponent(
    AbstractContext&              context,
    State                         state,
    WaveAudioTakeRef              take,
    WaveAudioTakeComponentOptions options)
    : WaveAudioTakeComponent::base(
          context,
          move(state),
          move(take)),
      options(move(options))
{
  this->updateThumbnail();
}

WaveAudioTakeComponent::~WaveAudioTakeComponent() = default;


// TODO: in look and feel

void WaveAudioTakeComponent::drawWaveform(
    juce::Graphics&     g,
    te::SmartThumbnail& thumb,

    juce::Rectangle<int> area,
    juce::Colour         colour,

    te::EditTimeRange       time,
    const WaveAudioTakeRef& take)
{
  if (!take.isValid()) return;
  auto& clip = *take.clip;

  const float gain = clip.getGain();
  const float panGain = thumb.getNumChannels() == 1 ? 0.0f : clip.getPan();
  const float gainL = (gain - panGain);
  const float gainR = (gain + panGain);


  g.setColour(colour);

  if (clip.usesTimeStretchedProxy())
  {
    if (!thumb.isOutOfDate()) return;

    WaveAudioTakeComponent::drawChannels(
        g,
        thumb,
        move(area),

        move(time),
        false,

        clip.isLeftChannelActive(),
        clip.isRightChannelActive(),
        gainL,
        gainR);
  }
  else if (clip.getLoopLength() == 0)
  {
    WaveAudioTakeComponent::drawChannels(
        g,
        thumb,
        move(area),

        {time.getStart() * clip.getSpeedRatio(),
         time.getEnd() * clip.getSpeedRatio()},

        false,
        clip.isLeftChannelActive(),
        clip.isRightChannelActive(),
        gainL,
        gainR);
  }
}

void WaveAudioTakeComponent::drawChannels(
    juce::Graphics&     g,
    te::SmartThumbnail& thumb,

    juce::Rectangle<int> area,

    te::EditTimeRange time,
    bool              useHighRes,
    bool              useLeft,
    bool              useRight,
    float             leftGain,
    float             rightGain)
{
  // TODO: channels better
  if (useLeft &&
      useRight &&
      thumb.getNumChannels() > 1)
  {
    thumb.drawChannel(
        g,
        area.removeFromTop(
            area.getHeight() / 2),
        useHighRes,
        time,
        0,
        leftGain);

    thumb.drawChannel(
        g,
        move(area),
        useHighRes,
        time,
        1,
        rightGain);
  }
  else if (useLeft)
  {
    thumb.drawChannel(
        g,
        move(area),
        useHighRes,
        time,
        0,
        leftGain);
  }
  else if (useRight)
  {
    thumb.drawChannel(
        g,
        move(area),
        useHighRes,
        time,
        1,
        rightGain);
  }
}

void WaveAudioTakeComponent::updateThumbnail()
{
  auto clip = this->getClip();
  if (!clip) return;

  auto& engine = this->getContext().getEngine();

  auto projectItem =
      engine
          .getProjectManager()
          .getProjectItem(this->getTakeRef().audioFileId);
  if (!projectItem) return;

  auto file = projectItem->getSourceFile();
  if (!file.existsAsFile()) return;

  te::AudioFile audioFile(engine, file);
  if (!audioFile.isValid())
  {
    this->thumbnail = nullptr;
    return;
  }

  if (this->thumbnail == nullptr)
  {
    this->thumbnail =
        std::make_unique<te::SmartThumbnail>(
            engine,
            audioFile,
            *this,
            &clip->edit);
  }
  else
  {
    thumbnail->setNewFile(
        audioFile);
  }
}


// Component

void WaveAudioTakeComponent::resized()
{
}

void WaveAudioTakeComponent::paint(juce::Graphics& g)
{
  auto clipPtr = this->getClip();
  if (!clipPtr) return;
  auto& clip = *clipPtr;

  auto thumbPtr = this->thumbnail.get();
  if (!thumbPtr) return;
  auto& thumb = *thumbPtr;

  WaveAudioTakeComponent::drawWaveform(
      g,
      thumb,

      {0,
       0,
       this->getWidth(),
       this->getHeight()},
      this->findColour(ColourId::background)
          .withAlpha(0.5f),

      // TODO time better?
      {clip.getPosition().getStartOfSource(),
       clip.getPosition().getEnd()},
      this->getTakeRef());
}

BLOOPER_NAMESPACE_END
