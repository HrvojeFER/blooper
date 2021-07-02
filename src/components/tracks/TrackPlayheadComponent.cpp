#include <blooper/components/tracks/TrackPlayheadComponent.hpp>

#include <blooper/internal/abstract/const.hpp>
#include <blooper/internal/utils/style.hpp>

BLOOPER_NAMESPACE_BEGIN

TrackPlayheadComponent::TrackPlayheadComponent(
    AbstractContext&     context,
    State                state,
    JuceTrackRef         track,
    TrackPlayheadOptions options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options)),

      track(move(track)),

      colourId(ColourId::transparent)
{
  this->startTimerHz(defaultGuiTimerHz);
}

TrackPlayheadComponent::~TrackPlayheadComponent()
{
  this->stopTimer();
}


// Component

void TrackPlayheadComponent::paint(JuceGraphics& g)
{
  g.fillAll(
      this->findColour(
          ColourId::foreground));

  g.setColour(
      this->findColour(this->colourId));

  // TODO: fix
  //  g.drawRect(
  //      static_cast<int>(
  //          this->track->getProgress() *
  //          this->getWidth()),
  //      0,
  //      2,
  //      this->getHeight());
}


// Timer

void TrackPlayheadComponent::timerCallback()
{
}

BLOOPER_NAMESPACE_END
