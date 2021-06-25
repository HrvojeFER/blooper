#include <blooper/components/tracks/TrackPlayheadComponent.hpp>

#include <blooper/internal/abstract/const.hpp>
#include <blooper/internal/utils/EditTrack.hpp>
#include <blooper/internal/utils/style.hpp>

BLOOPER_NAMESPACE_BEGIN

TrackPlayheadComponent::TrackPlayheadComponent(
    AbstractContext&     context,
    State                state,
    EditTrackRef         track,
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
      this->getLookAndFeel().findColour(
          this->colourId));

  g.drawRect(
      static_cast<int>(
          this->track->getProgress() *
          this->getWidth()),
      0,
      2,
      this->getHeight());
}


// Timer

void TrackPlayheadComponent::timerCallback()
{
  if (this->track->playback == TrackPlayback::paused)
  {
    this->colourId = ColourId::blue;
  }

  else if (this->track->playback == TrackPlayback::scheduledRecording)
  {
    this->colourId = ColourId::redBright;
  }
  else if (this->track->playback == TrackPlayback::recording)
  {
    this->colourId = ColourId::red;
  }

  else if (this->track->playback == TrackPlayback::scheduledPlaying)
  {
    this->colourId = ColourId::yellowBright;
  }
  else if (this->track->playback == TrackPlayback::playing)
  {
    this->colourId = ColourId::yellow;
  }

  this->repaint();
}

BLOOPER_NAMESPACE_END
