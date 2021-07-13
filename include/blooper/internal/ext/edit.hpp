#ifndef BLOOPER_EXT_EDIT_HPP
#define BLOOPER_EXT_EDIT_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/id.hpp>
#include <blooper/internal/abstract/meta.hpp>
#include <blooper/internal/abstract/predicates.hpp>
#include <blooper/internal/abstract/visitors.hpp>
#include <blooper/internal/ext/track.hpp>

BLOOPER_EXT_NAMESPACE_BEGIN

// Load

[[maybe_unused]] inline std::shared_ptr<te::Edit> loadEditFromItem(
    te::Engine&      engine,
    te::ProjectItem& item)
{
  BLOOPER_ASSERT(item.isEdit());

  std::shared_ptr edit{
      te::loadEditFromFile(
          engine,
          item.getSourceFile())};

  edit->setProjectItemID(item.getID());

  return edit;
}


// Visit

template<VisitDepth Depth = defaultVisitDepth, typename TVisitor>
[[maybe_unused]] inline bool visit(te::Edit& edit, TVisitor visitor)
{
  static_assert(
      BLOOPER_TYPEID(visitor) ^
          isVisitorOf ^
          meta::type_c<te::Track&>,
      "te::Edit visit requires a Visitor of te::Track&");

  for (auto track : edit.getTrackList().objects)
  {
    if (track)
    {
      if (!callVisitor(visitor, *track))
        return stopVisit;

      if constexpr (Depth == VisitDepth::deep)
        if (!visit<Depth>(*track, visitor))
          return stopVisit;
    }
  }

  return continueVisit;
}

template<VisitDepth Depth = defaultVisitDepth, typename TPredicate>
[[maybe_unused]] inline te::Track* find(te::Edit& edit, TPredicate predicate)
{
  static_assert(
      BLOOPER_TYPEID(predicate) ^
          isPredicateOf ^
          meta::type_c<te::Track&>,
      "te::Edit visit requires a Visitor of te::Track&");


  te::Track* result;

  visit<Depth>(
      edit,
      [&result, predicate = move(predicate)](
          te::Track& track) {
        if (predicate(track))
        {
          result = std::addressof(track);
          return stopVisit;
        }

        return continueVisit;
      });

  return result;
}


// Transport

[[maybe_unused]] inline auto
getPosition(const te::TransportControl& transport)
{
  return transport.getCurrentPosition();
}

[[maybe_unused]] inline auto
getPosition(const te::Edit& edit)
{
  return edit.getTransport().getCurrentPosition();
}

[[maybe_unused]] inline auto
getPositionBeats(const te::TransportControl& transport)
{
  return transport.edit.tempoSequence.timeToBeats(
      transport.getCurrentPosition());
}

[[maybe_unused]] inline auto
getPositionBeats(const te::Edit& edit)
{
  return edit.tempoSequence.timeToBeats(
      edit.getTransport().getCurrentPosition());
}


[[maybe_unused]] inline auto
setPosition(te::TransportControl& transport, double time)
{
  return transport.setCurrentPosition(time);
}

[[maybe_unused]] inline auto
setPosition(const te::Edit& edit, double time)
{
  return edit.getTransport().setCurrentPosition(time);
}

[[maybe_unused]] inline auto
setPositionBeats(te::TransportControl& transport, double beats)
{
  return transport.setCurrentPosition(
      transport.edit.tempoSequence.beatsToTime(beats));
}

[[maybe_unused]] inline auto
setPositionBeats(const te::Edit& edit, double beats)
{
  return edit.getTransport().setCurrentPosition(
      edit.tempoSequence.beatsToTime(beats));
}


[[maybe_unused]] inline bool isPlaying(const te::Edit& edit)
{
  return edit.getTransport().isPlaying();
}

[[maybe_unused]] void togglePlaying(
    te::Edit& edit,
    te::Edit* master = nullptr);


[[maybe_unused]] inline bool isRecording(const te::Edit& edit)
{
  return edit.getTransport().isRecording();
}

[[maybe_unused]] void toggleRecording(
    te::Edit& edit,
    te::Edit* master = nullptr);


// Tracks

[[maybe_unused]] inline te::AudioTrack::Ptr addAudioTrack(
    te::Edit&             edit,
    te::SelectionManager* selection = nullptr)
{
  auto audioTrack =
      edit.insertNewAudioTrack(
          {nullptr,
           nullptr},
          selection);

  init(*audioTrack);

  return move(audioTrack);
}

BLOOPER_EXT_NAMESPACE_END

#endif // BLOOPER_EXT_EDIT_HPP
