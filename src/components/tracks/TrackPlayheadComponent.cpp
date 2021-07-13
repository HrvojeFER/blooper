#include <blooper/components/tracks/TrackPlayheadComponent.hpp>

#include <blooper/internal/abstract/const.hpp>
#include <blooper/internal/ext/track.hpp>
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
  g.setColour(
      this->findColour(this->colourId));

  g.drawRect(
      static_cast<int>(
          ext::getProgress(*this->track) *
          this->getWidth()),
      0,
      2,
      this->getHeight());
}


// Timer

void TrackPlayheadComponent::timerCallback()
{
  auto& transport = this->track->edit.getTransport();
  if (transport.isPlaying())
  {
    if (transport.isRecording() &&
        ext::isArmed(*this->track))
    {
      this->colourId = ColourId::red;
    }
    else
    {
      this->colourId = ColourId::green;
    }
  }
  else
  {
    this->colourId = ColourId::blue;
  }

  this->repaint();
}

BLOOPER_NAMESPACE_END
