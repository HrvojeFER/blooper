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
                        meta::traits::is_convertible_to ^
                        meta::type_c<bool>) {}) ^
    meta::after ^
    meta::traits::has_result;

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

      static_assert(
          decltype(meta::is_concept(c)){},
          "makeConceptPredicate requires a concept");

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

      static_assert(
          decltype(meta::is_type(t)){},
          "makeTypePredicate requires a type");

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

      static_assert(
          decltype(meta::is_type(t)){},
          "makeTypePredicate requires a type");

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
           e = BLOOPER_FORWARD(e)](auto&& o) mutable {
            return e(BLOOPER_FORWARD(o), v);
          };
    });


// Pipes

[[maybe_unused]] inline constexpr auto makeStaticPipe =
    ([](auto&& t) {
      maybeUnused(BLOOPER_FORWARD(t));

      static_assert(
          decltype(meta::is_type(t)){},
          "makeStaticPipe requires a type");

      return
          [](auto&& o) {
            return static_cast<decltype(meta::source(BLOOPER_FORWARD(t)))>(
                BLOOPER_FORWARD(o));
          };
    });

[[maybe_unused]] inline constexpr auto makeDynamicPipe =
    ([](auto&& t) {
      maybeUnused(BLOOPER_FORWARD(t));

      static_assert(
          decltype((meta::traits::is_pointer ^
                    meta::after ^
                    meta::is_type)(t)){},
          "makeDynamicPipe requires a pointer type");

      return
          [](auto o) {
            return dynamic_cast<decltype(meta::source(BLOOPER_FORWARD(t)))>(o);
          };
    });

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PREDICATES_HPP
