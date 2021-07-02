#include <blooper/components/tracks/TrackButtonComponent.hpp>

#include <blooper/internal/abstract/id.hpp>
#include <blooper/internal/ext/component.hpp>
#include <blooper/internal/ext/track.hpp>
#include <blooper/internal/ext/clip.hpp>

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

    parentTrack.setMute(!parentTrack.isMuted(false));
  }

  void clicked(const juce::ModifierKeys&) final
  {
    auto& parentTrack = *this->parent->track;
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
}


// Component

void TrackButtonComponent::resized()
{
  auto availableArea = this->getLocalBounds();

  this->pimpl->setBounds(availableArea);
}


// ValueTreeListener

void TrackButtonComponent::valueTreePropertyChanged(
    juce::ValueTree& tree,
    const juce::Identifier&)
{
  if (tree == this->track->state)
  {
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
