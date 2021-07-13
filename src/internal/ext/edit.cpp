#include <blooper/internal/ext/edit.hpp>

BLOOPER_EXT_NAMESPACE_BEGIN

// Transport

void togglePlaying(te::Edit& edit, te::Edit* master)
{
  auto& transport = edit.getTransport();
  if (transport.isPlaying())
  {
    transport.stop(
        transport.isRecording(),
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

    if (master)
      transport.syncToEdit(
          master,
          false);
  }
}

void toggleRecording(te::Edit& edit, te::Edit* master)
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
    {
      if (master)
        transport.syncToEdit(
            master,
            false);

      transport.play(
          false);
    }
  }
  else
  {
    const auto wasPlaying = transport.isPlaying();

    if (wasPlaying)
    {
      transport.stop(
          true,
          false);
    }

    if (master)
      transport.syncToEdit(
          master,
          false);

    transport.record(
        false);

    if (wasPlaying)
    {
      transport.play(
          false);
    }
  }
}

BLOOPER_EXT_NAMESPACE_END
