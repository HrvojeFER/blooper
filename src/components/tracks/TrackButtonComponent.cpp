#include <blooper/components/tracks/TrackButtonComponent.hpp>

#include <blooper/internal/abstract/id.hpp>
#include <blooper/internal/ext/component.hpp>

#include <blooper/context/behaviour/AssetManager.hpp>
#include <blooper/context/behaviour/EditManager.hpp>

BLOOPER_NAMESPACE_BEGIN

class TrackButtonComponent::Pimpl final :
    public juce::DrawableButton
{
 public:
  explicit Pimpl(TrackButtonComponent* parent)
      : juce::DrawableButton(
            parent->track->getAudio().getName() + " Button",
            juce::DrawableButton::ImageFitted),
        parent(parent)
  {
  }

  ~Pimpl() override = default;


 private:
  TrackButtonComponent* parent;


  // Button

 public:
  void clicked() final
  {
    auto& parentTrack = *this->parent->track;

    if (parentTrack.playback == TrackPlayback::playing)
    {
      parentTrack.playback = TrackPlayback::paused;
    }
    else if (parentTrack.playback == TrackPlayback::recording)
    {
      parentTrack.playback = TrackPlayback::paused;
    }
    else if (parentTrack.playback == TrackPlayback::paused)
    {
      if (parentTrack.isClear())
      {
        parentTrack.playback = TrackPlayback::scheduledRecording;
      }
      else
      {
        parentTrack.playback = TrackPlayback::scheduledPlaying;
      }
    }
  }

  void clicked(const juce::ModifierKeys& modifiers) final
  {
    auto withCommand = modifiers.isCommandDown();

    if (withCommand)
    {
      this->parent->track->clear();
      this->parent->markAndUpdate(this->parent->imageUpdate);
    }
    else if (!modifiers.isAnyModifierKeyDown())
    {
      this->clicked();
    }
  }
};


TrackButtonComponent::TrackButtonComponent(
    AbstractContext&   context,
    State              state,
    EditTrackRef       track,
    TrackButtonOptions options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options)),

      track(move(track)),

      imageUpdate(false)
{
  this->pimpl =
      std::make_unique<TrackButtonComponent::Pimpl>(
          this);

  this->updateImages();


  ext::addAndMakeVisible(
      *this,
      *this->pimpl);


  this->track->getState().addListener(this);
  this->getContext().getEditManager().addListener(this);
}

TrackButtonComponent::~TrackButtonComponent()
{
  this->getContext().getEditManager().removeListener(this);
  this->track->getState().removeListener(this);
}


void TrackButtonComponent::updateImages()
{
  auto& assets = this->getContext().getAssetManager();

  const JuceDrawable* stateImage = nullptr;
  const JuceDrawable* nextStateImage = nullptr;

  if (this->track->playback == TrackPlayback::paused)
  {
    stateImage = assets.getIconView(assets::IconAssetId::pause);

    if (this->track->isClear())
    {
      nextStateImage = assets.getIconView(assets::IconAssetId::record);
    }
    else
    {
      nextStateImage = assets.getIconView(assets::IconAssetId::play);
    }
  }

  else if (this->track->playback == TrackPlayback::scheduledRecording)
  {
    stateImage = assets.getIconView(assets::IconAssetId::scheduledRecord);
    nextStateImage = assets.getIconView(assets::IconAssetId::pause);
  }
  else if (this->track->playback == TrackPlayback::recording)
  {
    stateImage = assets.getIconView(assets::IconAssetId::record);
    nextStateImage = assets.getIconView(assets::IconAssetId::pause);
  }

  else if (this->track->playback == TrackPlayback::scheduledPlaying)
  {
    stateImage = assets.getIconView(assets::IconAssetId::scheduledPlay);
    nextStateImage = assets.getIconView(assets::IconAssetId::pause);
  }
  else if (this->track->playback == TrackPlayback::playing)
  {
    stateImage = assets.getIconView(assets::IconAssetId::play);
    nextStateImage = assets.getIconView(assets::IconAssetId::pause);
  }

  this->pimpl->setImages(
      stateImage,
      nextStateImage,
      nextStateImage,
      nullptr,
      stateImage,
      nextStateImage,
      nextStateImage,
      nullptr);
}


// Component

void TrackButtonComponent::resized()
{
  auto availableArea = this->getLocalBounds();

  this->pimpl->setBounds(availableArea);
}


// ValueTreeListener

void TrackButtonComponent::valueTreePropertyChanged(
    juce::ValueTree&        tree,
    const juce::Identifier& id)
{
  auto& editManager = this->getContext().getEditManager();

  if (tree == this->track->getState())
  {
    if (id == id::playback)
    {
      this->markAndUpdate(this->imageUpdate);
    }
    else if (id == id::interval)
    {
    }
    else if (id == id::mode)
    {
    }
  }
  else if (tree == editManager.getState())
  {
    if (id == id::soloed)
    {
      this->markAndUpdate(this->imageUpdate);
    }
  }
}


// FlaggedAsyncUpdater

void TrackButtonComponent::handleAsyncUpdate()
{
  if (util::FlaggedAsyncUpdater::compareAndReset(imageUpdate))
  {
    this->updateImages();
  }
}

BLOOPER_NAMESPACE_END
