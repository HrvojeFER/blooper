#ifndef BLOOPER_DESKTOP_HPP
#define BLOOPER_DESKTOP_HPP
#pragma once

#include <blooper/internal/macros/namespaces.hpp>
#include <blooper/internal/abstract/predicates.hpp>
#include <blooper/internal/abstract/visitors.hpp>
#include <blooper/internal/ext/component.hpp>


BLOOPER_EXT_NAMESPACE_BEGIN

template<VisitDepth Depth = defaultVisitDepth, typename TVisitor>
[[maybe_unused]] inline bool
visit(const juce::Desktop& desktop, TVisitor visitor) noexcept(noexcept(
    visitor(std::declval<juce::Component&>())))
{
  static_assert(
      BLOOPER_TYPEID(visitor) ^
      isVisitorOf ^
      meta::type_c<juce::Component&>,
      "juce::Desktop visit requires a Visitor of juce::Component&");


  for (int i = 0; i < desktop.getNumComponents(); ++i)
  {
    if (auto component = desktop.getComponent(i))
    {
      if (callVisitor(visitor, *component) == stopVisit)
        return stopVisit;

      if constexpr (Depth == VisitDepth::deep)
        if (visit(*component, visitor) == stopVisit)
          return stopVisit;
    }
  }

  return continueVisit;
}

template<VisitDepth Depth = defaultVisitDepth, typename TPredicate>
[[maybe_unused, nodiscard]] inline juce::Component*
find(const juce::Desktop& desktop, TPredicate predicate) noexcept(noexcept(
    predicate(std::declval<juce::Component&>())))
{
  static_assert(
      BLOOPER_TYPEID(predicate) ^
      isPredicateOf ^
      meta::type_c<juce::Component&>,
      "juce::Desktop find requires a Predicate of juce::Component&");


  juce::Component* result;

  visit<Depth>(
      desktop,
      [&result, predicate = move(predicate)](
          juce::Component& node) {
        if (predicate(node))
        {
          result = std::addressof(node);
          return stopVisit;
        }

        return continueVisit;
      });

  return result;
}


template<VisitDepth Depth = defaultVisitDepth, typename TVisitor>
[[maybe_unused]] inline bool
visitComponents(TVisitor visitor) noexcept(noexcept(
    visitor(std::declval<juce::Component&>())))
{
  static_assert(
      BLOOPER_TYPEID(visitor) ^
          isVisitorOf ^
          meta::type_c<juce::Component&>,
      "visitComponents requires a Visitor of juce::Component&");

  return visit<Depth>(juce::Desktop::getInstance(), move(visitor));
}

template<VisitDepth Depth = defaultVisitDepth, typename TPredicate>
[[maybe_unused, nodiscard]] inline juce::Component*
findComponent(TPredicate predicate) noexcept(noexcept(
    predicate(std::declval<juce::Component&>())))
{
  static_assert(
      BLOOPER_TYPEID(predicate) ^
          isPredicateOf ^
          meta::type_c<juce::Component&>,
      "findComponent requires a Predicate of juce::Component&");

  return find<Depth>(juce::Desktop::getInstance(), move(predicate));
}

BLOOPER_EXT_NAMESPACE_END

#endif //BLOOPER_DESKTOP_HPP
