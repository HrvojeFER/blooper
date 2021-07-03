#ifndef BLOOPER_EXT_COMPONENT_HPP
#define BLOOPER_EXT_COMPONENT_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/traits.hpp>

BLOOPER_EXT_NAMESPACE_BEGIN

template<typename... TComponents>
[[maybe_unused]] inline auto addAndMakeVisible(
    juce::Component& to,
    TComponents&... components) noexcept(noexcept((to.addAndMakeVisible(components), ...)))
    -> decltype((to.addAndMakeVisible(components), ...))
{
  return (to.addAndMakeVisible(components), ...);
}


template<VisitDepth Depth = defaultVisitDepth, typename TVisitor>
[[maybe_unused]] inline bool
visit(const juce::Component& component, TVisitor visitor) noexcept(noexcept(
    visitor(std::declval<juce::Component&>())))
{
  static_assert(
      BLOOPER_TYPEID(visitor) ^ isVisitorOf ^ meta::type_c<juce::Component&>,
      "juce::Component visit requires a Visitor of juce::Component&");


  for (auto childPtr : component.getChildren())
  {
    if (childPtr)
    {
      auto& child = *childPtr;

      if (callVisitor(visitor, child) == stopVisit)
        return stopVisit;

      if constexpr (Depth == VisitDepth::deep)
        if (visit(child, visitor) == stopVisit)
          return stopVisit;
    }
  }

  return continueVisit;
}

template<typename TVisitor>
[[maybe_unused]] inline bool
visitAncestors(const juce::Component& component, TVisitor visitor) noexcept(noexcept(
    visitor(std::declval<juce::Component&>())))
{
  static_assert(
      BLOOPER_TYPEID(visitor) ^ isVisitorOf ^ meta::type_c<juce::Component&>,
      "juce::Component visitAncestors requires a Visitor of juce::Component&");


  if (auto parent = component.getParentComponent())
  {
    if (!callVisitor(visitor, *parent)) return stopVisit;

    return visitAncestors(*parent, visitor);
  }

  return continueVisit;
}

template<VisitDepth Depth = defaultVisitDepth, typename TPredicate>
[[maybe_unused, nodiscard]] inline juce::Component*
find(const juce::Component& component, TPredicate predicate) noexcept(noexcept(
    predicate(std::declval<juce::Component&>())))
{
  static_assert(
      BLOOPER_TYPEID(predicate) ^ isPredicateOf ^ meta::type_c<juce::Component&>,
      "juce::Component find requires a Predicate of juce::Component&");


  juce::Component* result;

  visit<Depth>(
      component,
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

template<typename TPredicate>
[[maybe_unused, nodiscard]] inline juce::Component*
findAncestor(const juce::Component& component, TPredicate predicate) noexcept(noexcept(
    predicate(std::declval<juce::Component&>())))
{
  static_assert(
      BLOOPER_TYPEID(predicate) ^ isPredicateOf ^ meta::type_c<juce::Component&>,
      "juce::Component findAncestor requires a Predicate of juce::Component&");


  juce::Component* result;

  visitAncestors(
      component,
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


[[maybe_unused]] inline auto setHeight(
    juce::Component& _for,
    int              height)
{
  auto bounds = _for.getBounds();
  bounds.setHeight(height);
  _for.setBounds(bounds);
}

[[maybe_unused]] inline auto setWidth(
    juce::Component& _for,
    int              width)
{
  auto bounds = _for.getBounds();
  bounds.setWidth(width);
  _for.setBounds(bounds);
}

BLOOPER_EXT_NAMESPACE_END

#endif // BLOOPER_EXT_COMPONENT_HPP
