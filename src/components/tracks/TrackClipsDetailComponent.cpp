#include <blooper/components/tracks/TrackClipsDetailComponent.hpp>

#include <blooper/internal/ext/value_tree.hpp>

#include <blooper/components/tracks/TrackClipsDetailContentComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

TrackClipsDetailComponent::TrackClipsDetailComponent(
    AbstractContext&                 context,
    State                            state,
    JuceTrackRef                     track,
    TrackClipsDetailComponentOptions options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options)),

      track(move(track))
{
  ext::referTo(
      this->viewportScrollRangeStart,
      this->getState(),
      TrackClipsDetailComponent::viewportScrollRangeStartId,
      nullptr,
      0.0);

  ext::referTo(
      this->viewportScrollRangeEnd,
      this->getState(),
      TrackClipsDetailComponent::viewportScrollRangeEndId,
      nullptr,
      0.0);


  this->content =
      makeChild<TrackClipsDetailContentComponent>(
          *this);

  this->viewport =
      addJuceChild<juce::Viewport>(
          *this,
          "Track Clips Detail Viewport");

  this->viewport->setViewedComponent(
      this->content.get(),
      false);

  this->viewport->setScrollOnDragEnabled(
      true);

  this->viewport->setScrollBarsShown(
      true,
      false,
      true,
      false);

  this->viewport->setScrollBarPosition(
      false,
      true);


  if (this->viewportScrollRangeStart != 0.0 ||
      this->viewportScrollRangeEnd != 0.0)
  {
    this->viewport->getVerticalScrollBar()
        .setCurrentRange(
            {this->viewportScrollRangeStart,
             this->viewportScrollRangeEnd},
            juce::dontSendNotification);
  }
}

TrackClipsDetailComponent::~TrackClipsDetailComponent()
{
  auto scrollRange =
      this->viewport->getVerticalScrollBar()
          .getCurrentRange();

  this->viewportScrollRangeStart = scrollRange.getStart();
  this->viewportScrollRangeEnd = scrollRange.getEnd();
}


// Component

void TrackClipsDetailComponent::resized()
{
  auto availableArea = this->getLocalBounds();

  this->viewport->setBounds(availableArea);
}

BLOOPER_NAMESPACE_END
