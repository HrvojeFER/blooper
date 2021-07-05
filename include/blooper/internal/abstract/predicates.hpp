#ifndef BLOOPER_PREDICATES_HPP
#define BLOOPER_PREDICATES_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/prelude.hpp>
#include <blooper/internal/abstract/meta.hpp>
#include <blooper/internal/abstract/traits.hpp>

BLOOPER_NAMESPACE_BEGIN

// Checks

[[maybe_unused]] inline constexpr auto isPredicate =
    meta::attribute(
        [](auto&& f, auto&& a)
            -> decltype(meta::typeid_(BLOOPER_FORWARD(f)(BLOOPER_FORWARD(a))) ^
                        meta::traits::is ^
                        meta::type_c<bool>) {}) ^
    meta::after ^
    meta::check(
        [](auto&& f, auto&& a)
            -> decltype(BLOOPER_FORWARD(f)(BLOOPER_FORWARD(a))) {});

[[maybe_unused]] inline constexpr auto isPredicateOf =
    meta::infix(isPredicate);

[[maybe_unused]] inline constexpr auto isAnyPredicate =
    isPredicateOf ^ meta::type_c<meta::anything>;


// Predicates

[[maybe_unused]] inline constexpr auto makeArgumentPredicate =
    ([](auto&& f) {
      maybeUnused(BLOOPER_FORWARD(f));
      return
          [](auto&& o) {
            maybeUnused(BLOOPER_FORWARD(o));
            return decltype(meta::typeid_(f) ^
                            meta::traits::is_invokable_with ^
                            meta::typeid_(o)){};
          };
    });

[[maybe_unused]] inline constexpr auto makeConceptPredicate =
    ([](auto&& c) {
      maybeUnused(BLOOPER_FORWARD(c));
      return
          [](auto&& o) {
            maybeUnused(BLOOPER_FORWARD(o));
            return decltype(meta::typeid_(o) ^
                            meta::models_concept ^
                            BLOOPER_FORWARD(c)){};
          };
    });

[[maybe_unused]] inline constexpr auto makeTagPredicate =
    ([](auto&& t) {
      maybeUnused(BLOOPER_FORWARD(t));
      return
          [](auto&& o) {
            maybeUnused(BLOOPER_FORWARD(o));
            return decltype(meta::tag_of_(o) ^
                            meta::traits::is ^
                            BLOOPER_FORWARD(t)){};
          };
    });

[[maybe_unused]] inline constexpr auto makeTypePredicate =
    ([](auto&& t) {
      maybeUnused(BLOOPER_FORWARD(t));
      return
          [](auto&& o) {
            maybeUnused(BLOOPER_FORWARD(o));
            return decltype(meta::typeid_(o) ^
                            meta::traits::is ^
                            BLOOPER_FORWARD(t)){};
          };
    });

[[maybe_unused]] inline constexpr auto makeValuePredicate =
    ([](auto&& v, auto&& e = meta::equal) {
      return
          [v = BLOOPER_FORWARD(v),
           e = BLOOPER_FORWARD(e)](auto&& o) {
            return e(BLOOPER_FORWARD(o), v);
          };
    });


// Pipes

[[maybe_unused]] inline constexpr auto makeStaticPipe =
    ([](auto&& t) {
      maybeUnused(BLOOPER_FORWARD(t));
      return
          [](auto&& o) {
            return static_cast<decltype(meta::origin(BLOOPER_FORWARD(t)))>(
                BLOOPER_FORWARD(o));
          };
    });

[[maybe_unused]] inline constexpr auto makeDynamicPipe =
    ([](auto&& t) {
      maybeUnused(BLOOPER_FORWARD(t));
      return
          [](auto&& o) {
            return dynamic_cast<decltype(meta::origin(BLOOPER_FORWARD(t)))>(
                BLOOPER_FORWARD(o));
          };
    });

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PREDICATES_HPP
