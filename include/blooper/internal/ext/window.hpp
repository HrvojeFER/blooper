#ifndef BLOOPER_EXT_WINDOW_HPP
#define BLOOPER_EXT_WINDOW_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/predicates.hpp>
#include <blooper/internal/abstract/visitors.hpp>


BLOOPER_EXT_NAMESPACE_BEGIN

template<typename TVisitor>
[[maybe_unused]] inline bool
visitWindows(TVisitor visitor) noexcept(noexcept(
    visitor(std::declval<juce::TopLevelWindow&>())))
{
  static_assert(
      BLOOPER_TYPEID(visitor) ^
          isVisitorOf ^
          meta::type_c<juce::TopLevelWindow&>,
      "visitWindows requires a Visitor of juce::TopLevelWindow&");


  for (auto i = 0; i < juce::TopLevelWindow::getNumTopLevelWindows(); ++i)
    if (auto window = juce::TopLevelWindow::getTopLevelWindow(i))
      if (!callVisitor(visitor, *window))
        return stopVisit;

  return continueVisit;
}

template<typename TPredicate>
[[maybe_unused, nodiscard]] inline juce::TopLevelWindow*
findWindow(TPredicate predicate) noexcept(noexcept(
    predicate(std::declval<juce::TopLevelWindow&>())))
{
  static_assert(
      BLOOPER_TYPEID(predicate) ^
          isPredicateOf ^
          meta::type_c<juce::TopLevelWindow&>,
      "findWindow requires a Predicate of juce::TopLevelWindow&");


  juce::TopLevelWindow* result;

  visitWindows(
      [&result, predicate = move(predicate)](
          juce::TopLevelWindow& window) {
        if (predicate(window))
        {
          result = std::addressof(window);
          return stopVisit;
        }

        return continueVisit;
      });

  return result;
}

BLOOPER_EXT_NAMESPACE_END

#endif // BLOOPER_EXT_WINDOW_HPP
