#include <blooper/components/tracks/TrackClipsComponent.hpp>

#include <blooper/internal/utils/EditTrack.hpp>

BLOOPER_NAMESPACE_BEGIN

TrackClipsComponent::TrackClipsComponent(
    AbstractContext&  context,
    State             state,
    EditTrackRef      track,
    TrackClipsOptions options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options)),

      track(move(track))
{
}

TrackClipsComponent::~TrackClipsComponent() = default;


// Component

void TrackClipsComponent::resized()
{
}

BLOOPER_NAMESPACE_END
