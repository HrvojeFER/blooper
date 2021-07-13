#ifndef BLOOPER_MATCHERS_HPP
#define BLOOPER_MATCHERS_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/prelude.hpp>
#include <blooper/internal/abstract/meta.hpp>
#include <blooper/internal/abstract/traits.hpp>
#include <blooper/internal/abstract/predicates.hpp>

BLOOPER_NAMESPACE_BEGIN

// Predicate matchers

[[maybe_unused]] inline constexpr auto makeConstexprPredicateMatcher =
    ([](auto&& p, auto&& f) {
      maybeUnused(BLOOPER_FORWARD(p));

      return
          [f = BLOOPER_FORWARD(f)](auto&& o) {
            maybeUnused(f);

            if constexpr (decltype(BLOOPER_FORWARD(p)(BLOOPER_FORWARD(o))){})
              return meta::just_flat(f(BLOOPER_FORWARD(o)));
            else
              return meta::nothing;
          };
    });


[[maybe_unused]] inline constexpr auto makePredicateMatcher =
    ([](auto&& p, auto&& f) {
      return
          [p = BLOOPER_FORWARD(p),
           f = BLOOPER_FORWARD(f)](auto&& o) -> decltype(auto) {
            using ot = std::decay_t<decltype(f(BLOOPER_FORWARD(o)))>;

            if (p(BLOOPER_FORWARD(o)))
              return std::optional<ot>{f(BLOOPER_FORWARD(o))};
            else
              return std::optional<ot>{std::nullopt};
          };
    });

[[maybe_unused]] inline constexpr auto makePipeMatcher =
    ([](auto&& p, auto&& f) {
      return
          [p = BLOOPER_FORWARD(p),
           f = BLOOPER_FORWARD(f)](auto&& o) -> decltype(auto) {
            using ot = std::decay_t<decltype(f(p(BLOOPER_FORWARD(o))))>;

            if (auto&& pr = p(BLOOPER_FORWARD(o)))
              return std::optional<ot>{f(BLOOPER_FORWARD(pr))};
            else
              return std::optional<ot>{std::nullopt};
          };
    });


[[maybe_unused]] inline constexpr auto makeVoidPredicateMatcher =
    ([](auto&& p, auto&& f) {
      return
          [p = BLOOPER_FORWARD(p),
           f = BLOOPER_FORWARD(f)](auto&& o) -> decltype(auto) {
            using ot = std::decay_t<decltype(f(BLOOPER_FORWARD(o)))>;

            if (p(BLOOPER_FORWARD(o)))
              return f(BLOOPER_FORWARD(o));
            else
              return;
          };
    });

[[maybe_unused]] inline constexpr auto makeVoidPipeMatcher =
    ([](auto&& p, auto&& f) {
      return
          [p = BLOOPER_FORWARD(p),
           f = BLOOPER_FORWARD(f)](auto&& o) -> decltype(auto) {
            using ot = std::decay_t<decltype(f(p(BLOOPER_FORWARD(o))))>;

            if (auto&& pr = p(BLOOPER_FORWARD(o)))
              return f(BLOOPER_FORWARD(pr));
            else
              return;
          };
    });


[[maybe_unused]] inline constexpr auto makeConstexprFunctionPredicateMatcher =
    ([](auto&& p, auto&& f) {
      maybeUnused(BLOOPER_FORWARD(p));

      return
          [f = BLOOPER_FORWARD(f)](auto&& o) {
            maybeUnused(f);

            if constexpr (decltype(BLOOPER_FORWARD(p)(BLOOPER_FORWARD(o))){})
              return meta::just_flat(move(f));
            else
              return meta::nothing;
          };
    });

[[maybe_unused]] inline constexpr auto makeFunctionPredicateMatcher =
    ([](auto&& p, auto&& f) {
      maybeUnused(BLOOPER_FORWARD(p));

      return
          [p = BLOOPER_FORWARD(p),
           f = BLOOPER_FORWARD(f)](auto&& o) {
            using ot = std::decay_t<decltype(f)>;

            if (p(BLOOPER_FORWARD(o)))
              return std::optional<ot>{move(f)};
            else
              return std::optional<ot>{std::nullopt};
          };
    });


// Concrete matchers

[[maybe_unused]] inline constexpr auto makeConstexprArgumentMatcher =
    meta::demux(makeConstexprPredicateMatcher)(
        makeArgumentPredicate,
        meta::id);

[[maybe_unused]] inline constexpr auto makeConstexprConceptMatcher =
    meta::compose(makeConstexprPredicateMatcher, makeConceptPredicate);

[[maybe_unused]] inline constexpr auto makeConstexprTagMatcher =
    meta::compose(makeConstexprPredicateMatcher, makeTagPredicate);

[[maybe_unused]] inline constexpr auto makeConstexprTypeMatcher =
    meta::compose(makeConstexprPredicateMatcher, makeTypePredicate);


[[maybe_unused]] inline constexpr auto makeConceptMatcher =
    meta::compose(makePredicateMatcher, makeConceptPredicate);

[[maybe_unused]] inline constexpr auto makeTagMatcher =
    meta::compose(makePredicateMatcher, makeTagPredicate);

[[maybe_unused]] inline constexpr auto makeTypeMatcher =
    meta::compose(makePredicateMatcher, makeTypePredicate);

[[maybe_unused]] inline constexpr auto makeValueMatcher =
    meta::compose(
        makePredicateMatcher,
        meta::reverse_partial(makeValuePredicate, meta::equal));


[[maybe_unused]] inline constexpr auto makeStaticMatcher =
    meta::compose(makePipeMatcher, makeStaticPipe);

[[maybe_unused]] inline constexpr auto makeDynamicMatcher =
    meta::compose(makePipeMatcher, makeDynamicPipe);


[[maybe_unused]] inline constexpr auto makeVoidConceptMatcher =
    meta::compose(makeVoidPredicateMatcher, makeConceptPredicate);

[[maybe_unused]] inline constexpr auto makeVoidTagMatcher =
    meta::compose(makeVoidPredicateMatcher, makeTagPredicate);

[[maybe_unused]] inline constexpr auto makeVoidTypeMatcher =
    meta::compose(makeVoidPredicateMatcher, makeTypePredicate);

[[maybe_unused]] inline constexpr auto makeVoidValueMatcher =
    meta::compose(
        makeVoidPredicateMatcher,
        meta::reverse_partial(makeValuePredicate, meta::equal));


[[maybe_unused]] inline constexpr auto makeVoidStaticMatcher =
    meta::compose(makeVoidPipeMatcher, makeStaticPipe);

[[maybe_unused]] inline constexpr auto makeVoidDynamicMatcher =
    meta::compose(makeVoidPipeMatcher, makeDynamicPipe);


[[maybe_unused]] inline constexpr auto makeConstexprFunctionArgumentMatcher =
    meta::demux(makeConstexprFunctionPredicateMatcher)(
        makeArgumentPredicate,
        meta::id);

[[maybe_unused]] inline constexpr auto makeConstexprFunctionConceptMatcher =
    meta::compose(
        makeConstexprFunctionPredicateMatcher,
        makeConceptPredicate);

[[maybe_unused]] inline constexpr auto makeConstexprFunctionTagMatcher =
    meta::compose(
        makeConstexprFunctionPredicateMatcher,
        makeTagPredicate);

[[maybe_unused]] inline constexpr auto makeConstexprFunctionTypeMatcher =
    meta::compose(
        makeConstexprFunctionPredicateMatcher,
        makeTypePredicate);


[[maybe_unused]] inline constexpr auto makeFunctionConceptMatcher =
    meta::compose(
        makeFunctionPredicateMatcher,
        makeConceptPredicate);

[[maybe_unused]] inline constexpr auto makeFunctionTagMatcher =
    meta::compose(
        makeFunctionPredicateMatcher,
        makeTagPredicate);

[[maybe_unused]] inline constexpr auto makeFunctionTypeMatcher =
    meta::compose(
        makeFunctionPredicateMatcher,
        makeTypePredicate);

[[maybe_unused]] inline constexpr auto makeFunctionValueMatcher =
    meta::compose(
        makeFunctionPredicateMatcher,
        meta::reverse_partial(makeValuePredicate, meta::equal));


// Compose

[[maybe_unused]] inline constexpr auto matcherConstexprCompose =
    ([](auto&& first, auto&& second) -> decltype(auto) {
      return [first = BLOOPER_FORWARD(first),
              second = BLOOPER_FORWARD(second)](auto&& object) {
        constexpr auto first_result_type =
            BLOOPER_TYPEID(first(BLOOPER_FORWARD(object)));

        if constexpr (
            meta::is_optional(first_result_type))
        {
          if constexpr (
              decltype(meta::is_nothing(meta::origin(first_result_type))){})
            return second(BLOOPER_FORWARD(object));
          else
            return first(BLOOPER_FORWARD(object));
        }
        else
        {
          return first(BLOOPER_FORWARD(object));
        }
      };
    });

[[maybe_unused]] inline constexpr auto matcherCompose =
    ([](auto&& first, auto&& second) -> decltype(auto) {
      return [first = BLOOPER_FORWARD(first),
              second = BLOOPER_FORWARD(second)](auto&& object) {
        static_assert(
            meta::equal(
                BLOOPER_TYPEID(first(BLOOPER_FORWARD(object))),
                BLOOPER_TYPEID(second(BLOOPER_FORWARD(object)))),
            "Runtime matcher result types must match. "
            "Be careful when combining different types of matchers.");


        constexpr auto first_result_type =
            BLOOPER_TYPEID(first(BLOOPER_FORWARD(object)));

        if constexpr (meta::is_std_optional(first_result_type))
        {
          auto&& first_result = first(BLOOPER_FORWARD(object));

          if (!BLOOPER_FORWARD(first_result).has_value())
            return second(BLOOPER_FORWARD(object));
          else
            return BLOOPER_FORWARD(first_result);
        }
        else
        {
          return first(BLOOPER_FORWARD(object));
        }
      };
    });

[[maybe_unused]] inline constexpr auto matcherVoidCompose =
    ([](auto&& first, auto&& second) -> decltype(auto) {
      return [first = BLOOPER_FORWARD(first),
              second = BLOOPER_FORWARD(second)](auto&& object) {
        first(BLOOPER_FORWARD(object));
        second(BLOOPER_FORWARD(object));
      };
    });


// Fold

[[maybe_unused]] inline constexpr auto matcherConstexprFold =
    ([](auto&& first, auto&&... rest) -> decltype(auto) {
      return meta::fold_left(
          meta::make_tuple(BLOOPER_FORWARD(rest)...),
          BLOOPER_FORWARD(first),
          matcherConstexprCompose);
    });

[[maybe_unused]] inline constexpr auto matcherFold =
    ([](auto&& first, auto&&... rest) -> decltype(auto) {
      return meta::fold_left(
          meta::make_tuple(BLOOPER_FORWARD(rest)...),
          BLOOPER_FORWARD(first),
          matcherCompose);
    });

[[maybe_unused]] inline constexpr auto matcherVoidFold =
    ([](auto&& first, auto&&... rest) -> decltype(auto) {
      return [matchers = meta::make_tuple(
                  BLOOPER_FORWARD(first),
                  BLOOPER_FORWARD(rest)...)](auto&& object) {
        meta::for_each(
            matchers,
            [object = BLOOPER_FORWARD(object)](auto&& matcher) {
              matcher(object);
            });
      };
    });

BLOOPER_NAMESPACE_END

#endif //BLOOPER_MATCHERS_HPP
