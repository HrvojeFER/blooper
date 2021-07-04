#ifndef BLOOPER_META_HPP
#define BLOOPER_META_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>

BLOOPER_NAMESPACE_BEGIN

// ignores any maybe unused value warning
template<typename... Ts>
[[maybe_unused]] inline constexpr auto maybeUnused(Ts&&...) noexcept
{
}


// ignores Clang-tidy performance-move-const-arg - identical to std::move
template<typename T>
[[maybe_unused, nodiscard]] inline constexpr std::remove_reference_t<T>&&
move(T&& v) noexcept
{
  return static_cast<std::remove_reference_t<T>&&>(v);
}


// BLOOPER_FORWARD macro function - identical to std::forward

template<typename T>
[[maybe_unused, nodiscard]] inline constexpr T&&
forward(std::remove_reference_t<T>&& v) noexcept
{
  BLOOPER_STATIC_ASSERT(
      meta::traits::is_lvalue_reference(meta::type_c<decltype(v)>),
      "Casting an lvalue to a rvalue is undefined behaviour.");

  return static_cast<T&&>(v);
}

template<typename T>
[[maybe_unused, nodiscard]] inline constexpr T&&
forward(std::remove_reference_t<T>& v) noexcept
{
  return static_cast<T&&>(v);
}


// the name is useful for adding meaning
template<typename T>
[[maybe_unused]] inline constexpr auto toggle(T& v) noexcept(noexcept(
    v = !v))
    -> decltype(v = !v)
{
  return v = !v;
}

template<typename T1, typename T2>
[[maybe_unused]] inline constexpr auto
implies(T1&& v1, T2&& v2) noexcept(noexcept(
    static_cast<bool>(BLOOPER_FORWARD(v1)),
    void(),
    static_cast<bool>(BLOOPER_FORWARD(v2))))

    -> decltype(static_cast<bool>(BLOOPER_FORWARD(v1)),
                void(),
                static_cast<bool>(BLOOPER_FORWARD(v2)),
                void(),
                bool())
{
  const auto b1 = static_cast<bool>(BLOOPER_FORWARD(v1));
  const auto b2 = static_cast<bool>(BLOOPER_FORWARD(v2));

  return !(b1 && !b2);
}

BLOOPER_NAMESPACE_END

BLOOPER_META_NAMESPACE_BEGIN

// Special

struct anything
{
  anything() = default;

  template<typename T>
  // NOLINTNEXTLINE(google-explicit-constructor,bugprone-forwarding-reference-overload)
  anything(T&&)
  { }

  template<typename T>
  // NOLINTNEXTLINE(google-explicit-constructor)
  operator T()
  { }
};

template<typename T>
[[maybe_unused]] inline constexpr bool operator==(T&&, anything) noexcept
{
  return true;
}

template<typename T>
[[maybe_unused]] inline constexpr bool operator==(anything, T&&) noexcept
{
  return true;
}

template<typename T>
[[maybe_unused]] inline constexpr bool operator!=(T&&, anything) noexcept
{
  return false;
}

template<typename T>
[[maybe_unused]] inline constexpr bool operator!=(anything, T&&) noexcept
{
  return false;
}

[[maybe_unused]] inline constexpr auto consume =
    [](auto&&...) {};

[[maybe_unused]] inline constexpr auto hoard =
    [](auto&...) {};


// Types

[[maybe_unused]] inline constexpr auto origin =
    [](auto&& t) -> typename std::remove_reference_t<decltype(t)>::type {};

[[maybe_unused]] inline constexpr auto is_type =
    is_valid(origin);

BLOOPER_STATIC_ASSERT(is_type(type_c<bool>));
BLOOPER_STATIC_ASSERT(not_(is_type(1)));
BLOOPER_STATIC_ASSERT(and_(is_type(type_c<bool>), is_type(type_c<int>)));


[[maybe_unused]] inline constexpr auto tag_of_ =
    ([](auto&& t) -> type<tag_of_t<std::remove_reference_t<decltype(t)>>> {
      return {};
    });

template<typename T>
[[maybe_unused]] inline constexpr auto tag_of_c =
    type_c<tag_of_t<std::remove_reference_t<T>>>;

template<typename T>
[[maybe_unused]] inline constexpr auto typeid_c =
    type_c<std::remove_cv_t<std::remove_reference_t<T>>>;

BLOOPER_STATIC_ASSERT(
    equal(tag_of_(make_tuple()),
          type_c<tuple_tag>));

BLOOPER_STATIC_ASSERT(
    equal(tag_of_(int_c<1>),
          type_c<integral_constant_tag<int>>));


// Concepts

template<template<typename> class Concept>
struct concept_applicator
{
  template<typename T>
  using apply = Concept<T>;
};

template<template<typename> class Concept>
[[maybe_unused]] inline constexpr auto concept_applicator_c =
    concept_applicator<Concept>{};

[[maybe_unused]] inline constexpr auto is_concept_applicator =
    is_valid([](auto&& a)
                 -> typename std::remove_reference_t<decltype(a)>::
                     template apply<anything> {});

[[maybe_unused]] inline constexpr auto models_concept =
    ([](auto&& concept_applicator, auto&& type) {
      static_assert(
          decltype(is_concept_applicator(type)){},
          "models_concept requires a concept_applicator");

      static_assert(
          decltype(is_type(type)){},
          "models_concept requires a type");

      return
          typename std::remove_reference_t<decltype(concept_applicator)>::
              template apply<type>{};
    });


// Checks

[[maybe_unused]] inline constexpr auto check =
    ([](auto&& f) {
      return [](auto&&... ts) {
        static_assert(
            decltype(and_(is_type(BLOOPER_FORWARD(ts))...)){},
            "check requires types.");

        return decltype(is_valid(
            BLOOPER_FORWARD(f),
            origin(BLOOPER_FORWARD(ts))...)){};
      };
    });

BLOOPER_STATIC_ASSERT(
    check(
        [](auto&& toCheck)
            -> decltype(BLOOPER_FORWARD(toCheck) +
                        BLOOPER_FORWARD(toCheck)) {})(
        type_c<int>));


[[maybe_unused]] inline constexpr auto attribute =
    ([](auto&& f) {
      return [](auto&&... ts) {
        static_assert(
            decltype(and_(is_type(BLOOPER_FORWARD(ts))...)){},
            "attribute requires types.");

        return decltype(BLOOPER_FORWARD(f)(
            origin(BLOOPER_FORWARD(ts))...)){};
      };
    });

BLOOPER_STATIC_ASSERT(
    attribute(
        [](auto&& toCheck)
            -> decltype(traits::is_pointer(typeid_(toCheck))) {})(
        type_c<int*>));


// Combinations

[[maybe_unused]] inline constexpr auto complement_of =
    demux(not_);

[[maybe_unused]] inline constexpr auto nand_ =
    complement_of(and_);

BLOOPER_STATIC_ASSERT(nand_(true, false));
BLOOPER_STATIC_ASSERT(not_(nand_(true, true, true)));
BLOOPER_STATIC_ASSERT(nand_(false, false, false));

[[maybe_unused]] inline constexpr auto nor_ =
    complement_of(or_);

BLOOPER_STATIC_ASSERT(nor_(false, false));
BLOOPER_STATIC_ASSERT(not_(nor_(true, false, false)));
BLOOPER_STATIC_ASSERT(not_(nor_(true, true, true)));

[[maybe_unused]] inline constexpr auto xor_ =
    demux(and_)(or_, nand_);

BLOOPER_STATIC_ASSERT(xor_(true, false, true));
BLOOPER_STATIC_ASSERT(not_(xor_(true, true, true)));
BLOOPER_STATIC_ASSERT(not_(xor_(false, false, false)));

[[maybe_unused]] inline constexpr auto nxor_ =
    complement_of(xor_);

BLOOPER_STATIC_ASSERT(nxor_(true, true, true));
BLOOPER_STATIC_ASSERT(nxor_(false, false));
BLOOPER_STATIC_ASSERT(not_(nxor_(true, false, true)));
BLOOPER_STATIC_ASSERT(not_(nxor_(false, true, false)));


[[maybe_unused]] inline constexpr auto satisfies_all =
    demux(and_);

BLOOPER_STATIC_ASSERT(
    satisfies_all(traits::is_pointer, traits::is_const)(
        type_c<int* const>));

BLOOPER_STATIC_ASSERT(
    not_(satisfies_all(traits::is_pointer, traits::is_const)(
        type_c<int*>)));


[[maybe_unused]] inline constexpr auto doesnt_satisfy_all =
    demux(nand_);

BLOOPER_STATIC_ASSERT(
    doesnt_satisfy_all(traits::is_pointer, traits::is_const)(
        type_c<float*>));

BLOOPER_STATIC_ASSERT(
    doesnt_satisfy_all(traits::is_pointer, traits::is_const)(
        type_c<float>));

BLOOPER_STATIC_ASSERT(
    not_(doesnt_satisfy_all(traits::is_pointer, traits::is_const)(
        type_c<float* const>)));


[[maybe_unused]] inline constexpr auto satisfies_any =
    demux(or_);

BLOOPER_STATIC_ASSERT(
    satisfies_any(traits::is_pointer, traits::is_const)(
        type_c<int*>));

BLOOPER_STATIC_ASSERT(
    not_(satisfies_any(traits::is_pointer, traits::is_const)(
        type_c<int>)));


[[maybe_unused]] inline constexpr auto doesnt_satisfy_any =
    demux(nor_);

BLOOPER_STATIC_ASSERT(
    doesnt_satisfy_any(traits::is_pointer, traits::is_const)(
        type_c<float>));

BLOOPER_STATIC_ASSERT(
    not_(doesnt_satisfy_any(traits::is_pointer, traits::is_const)(
        type_c<float*>)));

BLOOPER_STATIC_ASSERT(
    not_(doesnt_satisfy_any(traits::is_pointer, traits::is_const)(
        type_c<float* const>)));


[[maybe_unused]] inline constexpr auto satisfies_some =
    demux(xor_);

BLOOPER_STATIC_ASSERT(
    satisfies_some(traits::is_pointer, traits::is_const)(
        type_c<float*>));

BLOOPER_STATIC_ASSERT(
    not_(satisfies_some(traits::is_pointer, traits::is_const)(
        type_c<float* const>)));

BLOOPER_STATIC_ASSERT(
    not_(satisfies_some(traits::is_pointer, traits::is_const)(
        type_c<float>)));


[[maybe_unused]] inline constexpr auto doesnt_satisfy_some =
    demux(nxor_);

BLOOPER_STATIC_ASSERT(
    doesnt_satisfy_some(traits::is_pointer, traits::is_const)(
        type_c<float* const>));

BLOOPER_STATIC_ASSERT(
    doesnt_satisfy_some(traits::is_pointer, traits::is_const)(
        type_c<float>));

BLOOPER_STATIC_ASSERT(
    not_(doesnt_satisfy_some(traits::is_pointer, traits::is_const)(
        type_c<float*>)));


[[maybe_unused]] inline constexpr auto after =
    infix(
        [](auto&& f, auto&& b) {
          return [](auto&&... ts) {
            static_assert(
                decltype(and_(is_type(BLOOPER_FORWARD(ts))...)){},
                "after requires types.");

            if constexpr (
                decltype(BLOOPER_FORWARD(b)(
                    BLOOPER_FORWARD(ts)...)){})
              return decltype(BLOOPER_FORWARD(f)(
                  BLOOPER_FORWARD(ts)...)){};
            else
              return false_c;
          };
        });

BLOOPER_STATIC_ASSERT(
    not_((attribute(
              [](auto&& toCheck)
                  -> decltype(traits::is_same(
                      typeid_(toCheck()),
                      type_c<bool>)) {}) ^
          after ^
          check(
              [](auto&& toCheck)
                  -> decltype(toCheck()) {}))(
        type_c<int>) ));

BLOOPER_STATIC_ASSERT(
    (attribute(
         [](auto&& first, auto&& second)
             -> decltype(traits::is_same(
                 typeid_(first + second),
                 type_c<int>)) {}) ^
     after ^
     check(
         [](auto&& first, auto&& second)
             -> decltype(first + second) {}))(
        type_c<int>,
        type_c<char>) );


[[maybe_unused]] inline constexpr auto mediate =
    ([](auto&& callee, auto&& mediator) {
      auto construct =
          demux(BLOOPER_FORWARD(mediator))(
              make_tuple);

      auto call =
          reverse_partial(
              unpack,
              BLOOPER_FORWARD(callee));

      return demux(call)(construct);
    });

BLOOPER_STATIC_ASSERT(
    mediate(
        overload(
            [](auto) { return true; },
            [](auto, auto) { return false; }),
        reverse_partial(drop_front, int_c<1>))(1, 2));


[[maybe_unused]] inline constexpr auto just_flat =
    ([](auto&& o) {
      if constexpr (decltype(equal(
                        BLOOPER_TAG_OF(o),
                        type_c<optional_tag>)){})
        return BLOOPER_FORWARD(o);
      else
        return just(o);
    });

[[maybe_unused]] inline constexpr auto matcher_compose =
    ([](auto&& first, auto&& second) -> decltype(auto) {
      [[maybe_unused]] constexpr auto is_constexpr_matcher =
          ([](auto&& matcher) -> decltype(auto) {
            return equal(
                type_c<optional_tag>,
                BLOOPER_TAG_OF(matcher(anything{})));
          });

      if constexpr (decltype(is_constexpr_matcher(first)){})
      {
        return [second = BLOOPER_FORWARD(second)](auto&& object) {
          using first_result_type = decltype(first(BLOOPER_FORWARD(object)));
          constexpr first_result_type first_result{};

          if constexpr (equal(first_result, nothing))
            return second(BLOOPER_FORWARD(object));
          else
            return first_result;
        };
      }
      else
      {
        return [first = BLOOPER_FORWARD(first),
                second = BLOOPER_FORWARD(second)](auto&& object) {
          auto&& first_result = first(BLOOPER_FORWARD(object));

          if (first_result == std::nullopt)
            return second(BLOOPER_FORWARD(object));
          else
            return BLOOPER_FORWARD(first_result);
        };
      }
    });

[[maybe_unused]] inline constexpr auto matcher_fold =
    ([](auto&& first, auto&&... rest) -> decltype(auto) {
      return fold_left(
          make_tuple(BLOOPER_FORWARD(rest)...),
          BLOOPER_FORWARD(first),
          matcher_compose);
    });

[[maybe_unused]] inline constexpr auto make_inferred_matcher =
    ([](auto&& f) -> decltype(auto) {
      return [f = BLOOPER_FORWARD(f)](auto&& object) {
        [[maybe_unused]] constexpr auto enable_callable_with_object =
            [](auto&& c, auto&& o) -> decltype(c(BLOOPER_FORWARD(o))) {};
        [[maybe_unused]] constexpr auto check_matched =
            is_valid(enable_callable_with_object);
        using matched = decltype(check_matched(f, BLOOPER_FORWARD(object)));

        if constexpr (matched{})
          return just_flat(f(BLOOPER_FORWARD(object)));
        else
          return nothing;
      };
    });


[[maybe_unused]] inline constexpr auto make_concept_matcher =
    ([](auto&& concept_applicator, auto&& f) -> decltype(auto) {
      return make_inferred_matcher(
          [f = BLOOPER_FORWARD(f)](auto&& object) {
            if constexpr (decltype(models_concept(
                              BLOOPER_FORWARD(concept_applicator),
                              BLOOPER_FORWARD(object))){})
              return just_flat(f(BLOOPER_FORWARD(object)));
            else
              return nothing;
          });
    });

[[maybe_unused]] inline constexpr auto make_tag_matcher =
    ([](auto&& tag, auto&& f) -> decltype(auto) {
      return make_inferred_matcher(
          [f = BLOOPER_FORWARD(f)](auto&& object) {
            if constexpr (decltype(equal(
                              BLOOPER_FORWARD(tag),
                              BLOOPER_TAG_OF(object))){})
              return just_flat(f(BLOOPER_FORWARD(object)));
            else
              return nothing;
          });
    });

[[maybe_unused]] inline constexpr auto make_type_matcher =
    overload(
        ([](auto&& type, auto&& f) {
          return make_inferred_matcher(
              [f = BLOOPER_FORWARD(f)](auto&& object) {
                if constexpr (decltype(equal(
                                  BLOOPER_FORWARD(type),
                                  BLOOPER_TYPEID(object))){})
                  return just_flat(f(BLOOPER_FORWARD(object)));
                else
                  return nothing;
              });
        }),
        make_inferred_matcher);


[[maybe_unused]] inline constexpr auto make_value_matcher =
    ([](auto&& value, auto&& f) -> decltype(auto) {
      return make_inferred_matcher(
          [value = BLOOPER_FORWARD(value),
           f = BLOOPER_FORWARD(f)](auto&& object) -> decltype(auto) {
            [[maybe_unused]] constexpr auto optional_typeid =
                BLOOPER_TYPEID(f(BLOOPER_FORWARD(object)));
            using optional_type [[maybe_unused]] =
                typename decltype(optional_typeid)::type;

            if (value == BLOOPER_FORWARD(object))
              return std::optional<optional_type>{f(BLOOPER_FORWARD(object))};
            else
              return std::optional<optional_type>{std::nullopt};
          });
    });

[[maybe_unused]] inline constexpr auto make_dynamic_matcher =
    ([](auto&& type, auto&& f) -> decltype(auto) {
      return make_inferred_matcher(
          [f = BLOOPER_FORWARD(f)](auto&& object) -> decltype(auto) {
            [[maybe_unused]] constexpr auto optional_typeid =
                BLOOPER_TYPEID(f(std::declval<decltype(origin(type))>()));
            using optional_type [[maybe_unused]] =
                typename decltype(optional_typeid)::type;

            if (auto&& casted =
                    dynamic_cast<decltype(origin(type))>(
                        BLOOPER_FORWARD(object)))
              return std::optional<optional_type>{f(BLOOPER_FORWARD(casted))};
            else
              return std::optional<optional_type>{std::nullopt};
          });
    });

[[maybe_unused]] inline constexpr auto make_static_matcher =
    ([](auto&& type, auto&& f) -> decltype(auto) {
      return make_inferred_matcher(
          [f = BLOOPER_FORWARD(f)](auto&& object) -> decltype(auto) {
            [[maybe_unused]] constexpr auto optional_typeid =
                BLOOPER_TYPEID(f(std::declval<decltype(origin(type))>()));
            using optional_type [[maybe_unused]] =
                typename decltype(optional_typeid)::type;

            if (auto&& casted =
                    static_cast<decltype(origin(type))>(
                        BLOOPER_FORWARD(object)))
              return std::optional<optional_type>{f(BLOOPER_FORWARD(casted))};
            else
              return std::optional<optional_type>{std::nullopt};
          });
    });


BLOOPER_STATIC_ASSERT(
    equal(
        matcher_compose(
            make_type_matcher(
                typeid_(int_c<1>),
                [](auto&&) { return true_c; }),
            make_type_matcher(
                typeid_(int_c<2>),
                [](auto&&) { return false_c; }))(int_c<1>),
        just(true_c)));

BLOOPER_STATIC_ASSERT(
    equal(
        matcher_fold(
            make_type_matcher(
                typeid_(int_c<1>),
                [](auto&&) { return true_c; }),
            make_type_matcher(
                typeid_(int_c<2>),
                [](auto&&) { return false_c; }))(int_c<2>),
        just(false_c)));

BLOOPER_STATIC_ASSERT(
    equal(
        matcher_fold(
            make_tag_matcher(
                type_c<integral_constant_tag<int>>,
                [](auto&&) { return true_c; }),
            make_type_matcher(
                typeid_(int_c<2>),
                [](auto&&) { return false_c; }))(int_c<2>),
        just(true_c)));

BLOOPER_STATIC_ASSERT(
    equal(
        matcher_fold(
            make_value_matcher(
                int_c<2>,
                [](auto&&) { return true_c; }),
            make_type_matcher(
                typeid_(int_c<2>),
                [](auto&&) { return false_c; }))(int_c<2>),
        just(std::make_optional(true_c))));

BLOOPER_STATIC_ASSERT(
    equal(
        matcher_fold(
            make_type_matcher(
                typeid_(int_c<1>),
                [](auto&&) { return false; }))(int_c<2>),
        nothing));

BLOOPER_META_NAMESPACE_END
#endif //BLOOPER_META_HPP
