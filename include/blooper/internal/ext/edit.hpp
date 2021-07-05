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


// Armed

[[maybe_unused]] inline juce::Array<te::Track*> getArmedTracks(te::Edit& edit);


// Transport

[[maybe_unused]] inline bool isPlaying(te::Edit& edit)
{
  return edit.getTransport().isPlaying();
}

[[maybe_unused]] inline void togglePlaying(te::Edit& edit);


[[maybe_unused]] inline bool isRecording(te::Edit& edit)
{
  return edit.getTransport().isRecording();
}

[[maybe_unused]] void toggleRecording(te::Edit& edit);

BLOOPER_EXT_NAMESPACE_END

#endif // BLOOPER_EXT_EDIT_HPP
