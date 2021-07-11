#include <blooper/components/tracks/TrackButtonComponent.hpp>

#include <blooper/internal/abstract/id.hpp>
#include <blooper/internal/ext/component.hpp>
#include <blooper/internal/ext/track.hpp>

#include <blooper/context/behaviour/AssetManager.hpp>
#include <blooper/context/behaviour/EditManager.hpp>

BLOOPER_NAMESPACE_BEGIN

class TrackButtonComponent::Pimpl final :
    public juce::DrawableButton
{
 public:
  explicit Pimpl(TrackButtonComponent* parent)
      : juce::DrawableButton(
            parent->track->getName() + " Button",
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

    ext::toggleMuted(parentTrack);
  }

  void clicked(const juce::ModifierKeys& mods) final
  {
    auto& parentTrack = *this->parent->track;

    if (mods.isCommandDown())
    {
      ext::clear(parentTrack);
    }
    else if (mods.isAltDown())
    {
      ext::toggleArmed(parentTrack);
    }
    else if (mods.isShiftDown())
    {
      ext::toggleSoloed(parentTrack);
    }
    else if (!mods.isAnyModifierKeyDown())
    {
      this->clicked();
    }
  }
};


TrackButtonComponent::TrackButtonComponent(
    AbstractContext&   context,
    State              state,
    JuceTrackRef       track,
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


  this->track->state.addListener(this);
}

TrackButtonComponent::~TrackButtonComponent()
{
  this->track->state.removeListener(this);
}


void TrackButtonComponent::updateImages()
{
  auto& assets = this->getContext().getAssetManager();


  assets::IconAssetId iconId;
  assets::IconAssetId iconOverId;

  if (ext::isMuted(*this->track))
  {
    iconId = assets::IconAssetId::pause;
    iconOverId = assets::IconAssetId::play;
  }
  else
  {
    iconId = assets::IconAssetId::play;
    iconOverId = assets::IconAssetId::pause;
  }


  this->pimpl->setImages(
      assets.getIconView(iconId),
      assets.getIconView(iconOverId));
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
  if (tree == this->track->state)
  {
    if (id == te::IDs::mute)
    {
      this->markAndUpdate(this->imageUpdate);
    }
  }
}


// FlaggedAsyncUpdater

void TrackButtonComponent::handleAsyncUpdate()
{
  if (util::FlaggedAsyncUpdater::compareAndReset(this->imageUpdate))
  {
    this->updateImages();
  }
}

BLOOPER_NAMESPACE_END
