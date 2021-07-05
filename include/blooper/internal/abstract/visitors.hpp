#ifndef BLOOPER_VISITORS_HPP
#define BLOOPER_VISITORS_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/prelude.hpp>
#include <blooper/internal/abstract/meta.hpp>
#include <blooper/internal/abstract/traits.hpp>
#include <blooper/internal/abstract/predicates.hpp>

BLOOPER_NAMESPACE_BEGIN

// Depth

enum class VisitDepth
{
  deep,
  shallow,
};

[[maybe_unused]] inline constexpr auto defaultVisitDepth = VisitDepth::deep;


// Stop

[[maybe_unused]] inline constexpr auto continueVisit = true;
[[maybe_unused]] inline constexpr auto stopVisit = false;


// Traits

[[maybe_unused]] inline constexpr auto isStoppingVisitor =
    meta::attribute_wrap(
        [](auto&& f, auto&& a)
            -> decltype(BLOOPER_FORWARD(f) ^
                        isPredicateOf ^
                        BLOOPER_FORWARD(a)) {});

[[maybe_unused]] inline constexpr auto isStoppingVisitorOf =
    meta::infix(isStoppingVisitor);

[[maybe_unused]] inline constexpr auto isAnyStoppingVisitor =
    isStoppingVisitorOf ^ meta::type_c<meta::anything>;


[[maybe_unused]] inline constexpr auto isNonStoppingVisitor =
    meta::attribute_wrap(
        [](auto&& f, auto&& a)
            -> decltype(BLOOPER_FORWARD(f) ^
                        meta::traits::is_consumer_of ^
                        BLOOPER_FORWARD(a)) {});

[[maybe_unused]] inline constexpr auto isNonStoppingVisitorOf =
    meta::infix(isNonStoppingVisitor);

[[maybe_unused]] inline constexpr auto isAnyNonStoppingVisitor =
    isNonStoppingVisitorOf ^ meta::type_c<meta::anything>;


[[maybe_unused]] inline constexpr auto isVisitor =
    meta::satisfies_any(
        isStoppingVisitor,
        isNonStoppingVisitor);

[[maybe_unused]] inline constexpr auto isVisitorOf =
    meta::infix(isVisitor);


// Call

[[maybe_unused]] inline constexpr auto callVisitor =
    ([](auto&& v, auto&& i) {
      static_assert(
          BLOOPER_TYPEID(v) ^ isVisitorOf ^ BLOOPER_TYPEID(i),
          "callVisitor should receive a Visitor of the passed item");

      if constexpr (
          BLOOPER_TYPEID(v) ^ isAnyStoppingVisitor)
      {
        return (BLOOPER_FORWARD(v)(BLOOPER_FORWARD(i)));
      }
      else
      {
        (BLOOPER_FORWARD(v)(BLOOPER_FORWARD(i)));
        return continueVisit;
      }
    });

[[maybe_unused]] inline constexpr auto callIfVisitor =
    ([](auto&& v, auto&& i) {
      if constexpr (
          BLOOPER_TYPEID(v) ^ isVisitorOf ^ BLOOPER_TYPEID(i))
      {
        return callVisitor(BLOOPER_FORWARD(v), BLOOPER_FORWARD(i));
      }
      else
      {
        maybeUnused(BLOOPER_FORWARD(v), BLOOPER_FORWARD(i));
        return continueVisit;
      }
    });

BLOOPER_NAMESPACE_END

#endif // BLOOPER_VISITORS_HPP
