#include <blooper/internal/ext/edit.hpp>
#include <blooper/internal/ext/track.hpp>

BLOOPER_EXT_NAMESPACE_BEGIN

// Armed

[[maybe_unused]] inline juce::Array<te::Track*> getArmedTracks(
    te::Edit& edit)
{
  juce::Array<te::Track*> armedTracks{};

  edit.visitAllTopLevelTracks([&armedTracks](te::Track& track) {
    if (isArmed(track)) armedTracks.add(std::addressof(track));
    return true; // continue
  });

  return move(armedTracks);
}


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
    edit.visitAllTopLevelTracks([](te::Track& track) {
      prepareForPlaying(track);
      return true; // continue
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

    for (const auto track : getArmedTracks(edit))
      unpackRecordedTakes(*track);

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
