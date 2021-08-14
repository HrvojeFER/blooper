#include <blooper/components/takes/WaveAudioTakeComponent.hpp>

#include <blooper/internal/utils/gui.hpp>

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

  if (this->options.shouldResizeItself)
    this->getClip().state.addListener(this);
}

WaveAudioTakeComponent::~WaveAudioTakeComponent()
{
  this->getClip().state.removeListener(this);
}


// TODO: in look and feel?

void WaveAudioTakeComponent::drawWaveform(
    JuceGraphics&     g,
    JuceThumbnail&    thumb,
    WaveAudioTakeRef& take,
    JuceTimeRange     time,

    JuceBounds bounds,
    JuceColour colour)
{
  BLOOPER_ASSERT(take.isValid());
  auto& clip = *take.clip;

  const float gain = clip.getGain();
  const float panGain = thumb.getNumChannels() == 1 ? 0.0f : clip.getPan();
  const float gainL = (gain - panGain);
  const float gainR = (gain + panGain);


  g.setColour(colour);

  WaveAudioTakeComponent::drawChannels(
      g,
      thumb,
      move(time),

      move(bounds),
      false,
      clip.isLeftChannelActive(),
      clip.isRightChannelActive(),
      gainL,
      gainR);
}

void WaveAudioTakeComponent::drawChannels(
    JuceGraphics&  g,
    JuceThumbnail& thumb,
    JuceTimeRange  time,

    JuceBounds bounds,
    bool       useHighRes,
    bool       useLeft,
    bool       useRight,
    float      leftGain,
    float      rightGain)
{
  // TODO: channels better
  if (useLeft &&
      useRight &&
      thumb.getNumChannels() > 1)
  {
    thumb.drawChannel(
        g,
        move(bounds),
        useHighRes,
        move(time),
        0,
        leftGain);

    thumb.drawChannel(
        g,
        move(bounds),
        useHighRes,
        move(time),
        1,
        rightGain);
  }
  else if (useLeft)
  {
    thumb.drawChannel(
        g,
        move(bounds),
        useHighRes,
        move(time),
        0,
        leftGain);
  }
  else if (useRight)
  {
    thumb.drawChannel(
        g,
        move(bounds),
        useHighRes,
        move(time),
        1,
        rightGain);
  }
}

// TODO react
void WaveAudioTakeComponent::updateThumbnail()
{
  auto& clip = *this->getHeldTakeRef().clip;
  auto& edit = clip.edit;
  auto& engine = clip.edit.engine;

  auto projectItem =
      engine
          .getProjectManager()
          .getProjectItem(this->getHeldTakeRef().audioFileId);
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
            std::addressof(edit));
  }
  else
  {
    thumbnail->setNewFile(
        audioFile);
  }
}


// ValueTreeListener

void WaveAudioTakeComponent::valueTreePropertyChanged(
    JuceState&                 state,
    const JuceStateIdentifier& id)
{
  if (state == this->getClip().state)
  {
    if (id == te::IDs::offset ||
        id == te::IDs::start ||
        id == te::IDs::length)
    {
      this->setBounds(
          util::withHorizontalRange(
              this->getBounds(),
              this->getTimePixelMapping(
                      dynamic_cast<AbstractTimePixelConverter*>(
                          this->getParentComponent()))
                  .pixels));

      if (this->options.shouldResizeParentWhenResized &&
          this->getParentComponent())
        this->getParentComponent()->resized();
    }
  }
}

// Component

void WaveAudioTakeComponent::paint(juce::Graphics& g)
{
  const auto thumbPtr = this->thumbnail.get();
  if (!thumbPtr) return;
  auto& thumb = *thumbPtr;

  const auto mapping = this->getTimePixelMapping();

  WaveAudioTakeComponent::drawWaveform(
      g,
      thumb,
      this->getHeldTakeRef(),
      mapping.time,

      util::withHorizontalRange(this->getLocalBounds(), mapping.pixels),
      this->findColour(ColourId::yellow)
          .withAlpha(0.5f));
}

BLOOPER_NAMESPACE_END
