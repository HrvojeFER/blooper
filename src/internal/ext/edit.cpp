#include <blooper/internal/ext/edit.hpp>

BLOOPER_EXT_NAMESPACE_BEGIN

// Transport

void togglePlaying(te::Edit& edit)
{
  auto& transport = edit.getTransport();

  if (transport.isPlaying())
  {
    transport.stop(
        true,
        false);
  }
  else
  {
    visit<VisitDepth::shallow>(
        edit,
        [](te::Track& track) {
          prepareForPlaying(track);
        });

    transport.play(
        false);
  }
}

void toggleRecording(te::Edit& edit)
{
  auto& transport = edit.getTransport();

  if (transport.isRecording())
  {
    const auto wasPlaying = transport.isPlaying();

    transport.stop(
        false,
        false);

    visit<VisitDepth::shallow>(
        edit,
        [](te::Track& track) {
          if (isArmed(track))
            unpackRecordedTakes(track);
        });

    if (wasPlaying)
      transport.play(
          false);
  }
  else
  {
    transport.record(
        false);
  }
}

BLOOPER_EXT_NAMESPACE_END
