#ifndef BLOOPER_PARAMETER_HPP
#define BLOOPER_PARAMETER_HPP

#include <blooper/internal/macros/macros.hpp>

BLOOPER_EXT_NAMESPACE_BEGIN

[[maybe_unused]] inline void nudgeParameterUp(
    te::AutomatableParameter& parameter)
{
  parameter.setNormalisedParameter(
      parameter.getCurrentValue() +
          parameter.getValueRange().getLength() / 20,
      juce::sendNotificationAsync);
}

[[maybe_unused]] inline void nudgeParameterDown(
    te::AutomatableParameter& parameter)
{
  parameter.setNormalisedParameter(
      parameter.getCurrentValue() -
          parameter.getValueRange().getLength() / 20,
      juce::sendNotificationAsync);
}


// Visit

[[maybe_unused]] inline constexpr auto isMacroParameterVisitor =
    isVisitorOf ^ meta::type_c<te::MacroParameter&>;

[[maybe_unused]] inline constexpr auto isModifierVisitor =
    isVisitorOf ^ meta::type_c<te::Modifier&>;

template<VisitDepth Depth = defaultVisitDepth, typename TVisitor>
[[maybe_unused]] inline bool visit(
    te::AutomatableParameter& edit,
    TVisitor                  visitor)
{
  static_assert(
      BLOOPER_TYPEID(visitor) ^ isVisitorOf ^ meta::type_c<te::Track&>,
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
      BLOOPER_TYPEID(predicate) ^ isPredicateOf ^ meta::type_c<te::Track&>,
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

BLOOPER_EXT_NAMESPACE_END

#endif // BLOOPER_PARAMETER_HPP
