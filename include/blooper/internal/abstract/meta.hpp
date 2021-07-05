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

[[maybe_unused]] inline constexpr auto consume =
    [](auto&&...) {};

[[maybe_unused]] inline constexpr auto hoard =
    [](auto&...) {};


// Types

[[maybe_unused]] inline constexpr auto origin =
    ([](auto&& t) -> typename std::remove_reference_t<decltype(t)>::type {
      return {};
    });

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
struct concept_
{
  using type = concept_;

  template<typename T>
  using apply = Concept<T>;
};

template<template<typename> class Concept>
[[maybe_unused]] inline constexpr auto concept_c =
    concept_<Concept>{};

[[maybe_unused]] inline constexpr auto is_concept =
    is_valid([](auto&& c)
                 -> typename std::remove_reference_t<decltype(c)>::
                     template apply<anything> {});

[[maybe_unused]] inline constexpr auto models_concept =
    ([](auto&& c, auto&& type) {
      static_assert(
          decltype(is_concept(c)){},
          "models_concept requires a concept");

      static_assert(
          decltype(or_(is_type(type))){},
          "models_concept requires a type");

      return
          typename std::remove_reference_t<decltype(c)>::
              template apply<decltype(origin(type))>{};
    });

BLOOPER_STATIC_ASSERT(
    is_concept(
        concept_c<Searchable>));

BLOOPER_STATIC_ASSERT(
    not_(is_concept(
        int_c<1>)));

BLOOPER_STATIC_ASSERT(
    models_concept(
        concept_c<Searchable>,
        typeid_(make_tuple())));

BLOOPER_STATIC_ASSERT(
    not_(models_concept(
        concept_c<Searchable>,
        typeid_(int_c<1>))));

BLOOPER_STATIC_ASSERT(
    models_concept(
        concept_c<Searchable>,
        type_c<tuple_tag>));


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


// Optional

namespace optional_detail
{
struct is_std_optional_t
{
  template<typename T>
  [[maybe_unused]] inline constexpr auto
  operator()(T const&) const noexcept
  {
    return false_c;
  }

  template<typename T>
  [[maybe_unused]] inline constexpr auto
  operator()(std::optional<T> const&) const noexcept
  {
    return true_c;
  }
};

struct is_optional_t
{
  template<typename T>
  [[maybe_unused]] inline constexpr auto
  operator()(T const&) const noexcept
  {
    return false_c;
  }

  template<typename T>
  [[maybe_unused]] inline constexpr auto
  operator()(meta::optional<T> const&) const noexcept
  {
    return true_c;
  }

  [[maybe_unused]] inline constexpr auto
  operator()(meta::optional<> const&) const noexcept
  {
    return true_c;
  }
};

} // namespace optional_detail


[[maybe_unused]] inline constexpr auto is_std_optional =
    ([](auto&& t) {
      static_assert(
          decltype(is_type(BLOOPER_FORWARD(t))){},
          "is_std_optional requires a type");

      [[maybe_unused]] constexpr optional_detail::is_std_optional_t f{};
      return decltype(f(std::declval<decltype(origin(BLOOPER_FORWARD(t)))>())){};
    });

BLOOPER_STATIC_ASSERT(is_std_optional(typeid_(std::make_optional(1))));

BLOOPER_STATIC_ASSERT(not_(is_std_optional(typeid_(1))));


[[maybe_unused]] inline constexpr auto is_optional =
    ([](auto&& t) {
      static_assert(
          decltype(is_type(BLOOPER_FORWARD(t))){},
          "is_std_optional requires a type");

      [[maybe_unused]] constexpr optional_detail::is_optional_t f{};
      return decltype(f(std::declval<decltype(origin(t))>())){};
    });

BLOOPER_STATIC_ASSERT(is_optional(typeid_(just(true_c))));

BLOOPER_STATIC_ASSERT(is_optional(typeid_(nothing)));

BLOOPER_STATIC_ASSERT(not_(is_optional(typeid_(1))));


[[maybe_unused]] inline constexpr auto just_flat =
    ([](auto&& o) {
      constexpr auto t = BLOOPER_TYPEID(o);

      if constexpr (is_optional(t) || is_std_optional(t))
        return BLOOPER_FORWARD(o);
      else
        return just(BLOOPER_FORWARD(o));
    });

BLOOPER_STATIC_ASSERT(equal(just_flat(just(true_c)), just(true_c)));

BLOOPER_STATIC_ASSERT(equal(just_flat(true_c), just(true_c)));


// Matchers

[[maybe_unused]] inline constexpr auto make_constexpr_predicate_matcher =
    ([](auto&& p, auto&& f) {
      return
          [f = BLOOPER_FORWARD(f)](auto&& object) {
            if constexpr (decltype(p(BLOOPER_FORWARD(object))){})
              return just_flat(f(BLOOPER_FORWARD(object)));
            else
              return nothing;
          };
    });

[[maybe_unused]] inline constexpr auto make_constexpr_argument_matcher =
    ([](auto&& f) {
      return make_constexpr_predicate_matcher(
          [](auto&& o) {
            [[maybe_unused]] constexpr auto arguments_matched =
                is_valid([](auto&& f, auto&& o)
                             -> decltype(f(BLOOPER_FORWARD(o))) {});

            return decltype(arguments_matched(
                BLOOPER_FORWARD(f),
                BLOOPER_FORWARD(o))){};
          },
          BLOOPER_FORWARD(f));
    });

[[maybe_unused]] inline constexpr auto make_constexpr_concept_matcher =
    ([](auto&& c, auto&& f) {
      return make_constexpr_predicate_matcher(
          [](auto&& o) {
            return decltype(models_concept(
                BLOOPER_FORWARD(c),
                BLOOPER_FORWARD(o))){};
          },
          BLOOPER_FORWARD(f));
    });

[[maybe_unused]] inline constexpr auto make_constexpr_tag_matcher =
    ([](auto&& t, auto&& f) {
      return make_constexpr_predicate_matcher(
          [](auto&& o) {
            return decltype(decltype(equal(
                BLOOPER_FORWARD(t),
                BLOOPER_TAG_OF(o))){}){};
          },
          BLOOPER_FORWARD(f));
    });

[[maybe_unused]] inline constexpr auto make_constexpr_type_matcher =
    ([](auto&& t, auto&& f) {
      return make_constexpr_predicate_matcher(
          [](auto&& o) {
            return decltype(decltype(equal(
                BLOOPER_FORWARD(t),
                BLOOPER_TYPEID(o))){}){};
          },
          BLOOPER_FORWARD(f));
    });

BLOOPER_STATIC_ASSERT(
    equal(
        make_constexpr_predicate_matcher(
            [](auto&& o) { return !traits::is_pointer(typeid_(o)); },
            [](auto&&) { return true_c; })(int_c<1>),
        just(true_c)));

BLOOPER_STATIC_ASSERT(
    equal(
        make_constexpr_argument_matcher(
            [](int_<1>) { return true_c; })(int_c<1>),
        just(true_c)));

BLOOPER_STATIC_ASSERT(
    equal(
        make_constexpr_concept_matcher(
            concept_c<IntegralConstant>,
            [](auto&&) { return true_c; })(int_c<1>),
        just(true_c)));

BLOOPER_STATIC_ASSERT(
    equal(
        make_constexpr_tag_matcher(
            type_c<integral_constant_tag<int>>,
            [](auto&&) { return true_c; })(int_c<1>),
        just(true_c)));

BLOOPER_STATIC_ASSERT(
    equal(
        make_constexpr_tag_matcher(
            type_c<tuple_tag>,
            [](auto&&) { return true_c; })(int_c<1>),
        nothing));

BLOOPER_STATIC_ASSERT(
    equal(
        make_constexpr_type_matcher(
            type_c<int_<1>>,
            [](auto&&) { return true_c; })(int_c<1>),
        just(true_c)));

BLOOPER_STATIC_ASSERT(
    equal(
        make_constexpr_type_matcher(
            type_c<int_<2>>,
            [](auto&&) { return true_c; })(int_c<1>),
        nothing));


[[maybe_unused]] inline constexpr auto make_predicate_matcher =
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

[[maybe_unused]] inline constexpr auto make_value_matcher =
    ([](auto&& value, auto&& f) {
      return make_predicate_matcher(
          ([value = BLOOPER_FORWARD(value)](auto&& object) {
            return value == BLOOPER_FORWARD(object);
          }),
          BLOOPER_FORWARD(f));
    });

[[maybe_unused]] inline constexpr auto make_concept_matcher =
    ([](auto&& c, auto&& f) {
      return make_predicate_matcher(
          [](auto&& o) {
            return decltype(models_concept(
                BLOOPER_FORWARD(c),
                BLOOPER_FORWARD(o))){};
          },
          BLOOPER_FORWARD(f));
    });

[[maybe_unused]] inline constexpr auto make_tag_matcher =
    ([](auto&& t, auto&& f) {
      return make_predicate_matcher(
          [](auto&& o) {
            return decltype(decltype(equal(
                BLOOPER_FORWARD(t),
                BLOOPER_TAG_OF(o))){}){};
          },
          BLOOPER_FORWARD(f));
    });

[[maybe_unused]] inline constexpr auto make_type_matcher =
    ([](auto&& t, auto&& f) {
      return make_predicate_matcher(
          [](auto&& o) {
            return decltype(decltype(equal(
                BLOOPER_FORWARD(t),
                BLOOPER_TYPEID(o))){}){};
          },
          BLOOPER_FORWARD(f));
    });

BLOOPER_STATIC_ASSERT(
    equal(
        make_predicate_matcher(
            [](auto&& o) { return BLOOPER_FORWARD(o) == 1; },
            [](auto&&) { return true_c; })(int_c<1>),
        std::make_optional(true_c)));

BLOOPER_STATIC_ASSERT(
    equal(
        make_value_matcher(
            1,
            [](auto&&) { return true; })(1),
        std::optional<bool>(true)));

BLOOPER_STATIC_ASSERT(
    equal(
        make_value_matcher(
            2,
            [](auto&&) { return true; })(1),
        std::optional<bool>(std::nullopt)));

BLOOPER_STATIC_ASSERT(
    equal(
        make_concept_matcher(
            concept_c<IntegralConstant>,
            [](auto&&) { return true; })(int_c<1>),
        std::make_optional(true)));

BLOOPER_STATIC_ASSERT(
    equal(
        make_tag_matcher(
            type_c<integral_constant_tag<int>>,
            [](auto&&) { return true; })(int_c<1>),
        std::make_optional(true)));

BLOOPER_STATIC_ASSERT(
    equal(
        make_tag_matcher(
            type_c<tuple_tag>,
            [](auto&&) { return true; })(int_c<1>),
        std::optional<bool>{std::nullopt}));

BLOOPER_STATIC_ASSERT(
    equal(
        make_type_matcher(
            type_c<int_<1>>,
            [](auto&&) { return true; })(int_c<1>),
        std::make_optional(true)));

BLOOPER_STATIC_ASSERT(
    equal(
        make_type_matcher(
            type_c<int_<2>>,
            [](auto&&) { return true; })(int_c<1>),
        std::optional<bool>{std::nullopt}));


[[maybe_unused]] inline constexpr auto make_pipe_matcher =
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

[[maybe_unused]] inline constexpr auto make_static_matcher =
    ([](auto&& t, auto&& f) {
      return make_pipe_matcher(
          [](auto&& o) {
            return static_cast<decltype(origin(BLOOPER_FORWARD(t)))>(
                BLOOPER_FORWARD(o));
          },
          BLOOPER_FORWARD(f));
    });

[[maybe_unused]] inline constexpr auto make_dynamic_matcher =
    ([](auto&& t, auto&& f) {
      return make_pipe_matcher(
          [](auto&& o) {
            return dynamic_cast<decltype(origin(BLOOPER_FORWARD(t)))>(
                BLOOPER_FORWARD(o));
          },
          BLOOPER_FORWARD(f));
    });

BLOOPER_STATIC_ASSERT(
    equal(
        make_pipe_matcher(
            [](auto&& o) { return BLOOPER_FORWARD(o) - 1; },
            [](auto&&) { return true; })(int_c<1>),
        std::optional<bool>(std::nullopt)));

BLOOPER_STATIC_ASSERT(
    equal(
        make_static_matcher(
            type_c<int>,
            [](auto&& o) { return BLOOPER_FORWARD(o); })(1.0),
        std::make_optional(1)));

// TODO: dynamic matcher assert?


[[maybe_unused]] inline constexpr auto make_void_predicate_matcher =
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

[[maybe_unused]] inline constexpr auto make_void_value_matcher =
    ([](auto&& value, auto&& f) {
      return make_void_predicate_matcher(
          ([value = BLOOPER_FORWARD(value)](auto&& object) {
            return value == BLOOPER_FORWARD(object);
          }),
          BLOOPER_FORWARD(f));
    });

[[maybe_unused]] inline constexpr auto make_void_concept_matcher =
    ([](auto&& c, auto&& f) {
      return make_void_predicate_matcher(
          [](auto&& o) {
            return decltype(models_concept(
                BLOOPER_FORWARD(c),
                BLOOPER_FORWARD(o))){};
          },
          BLOOPER_FORWARD(f));
    });

[[maybe_unused]] inline constexpr auto make_void_tag_matcher =
    ([](auto&& t, auto&& f) {
      return make_void_predicate_matcher(
          [](auto&& o) {
            return decltype(decltype(equal(
                BLOOPER_FORWARD(t),
                BLOOPER_TAG_OF(o))){}){};
          },
          BLOOPER_FORWARD(f));
    });

[[maybe_unused]] inline constexpr auto make_void_type_matcher =
    ([](auto&& t, auto&& f) {
      return make_void_predicate_matcher(
          [](auto&& o) {
            return decltype(decltype(equal(
                BLOOPER_FORWARD(t),
                BLOOPER_TYPEID(o))){}){};
          },
          BLOOPER_FORWARD(f));
    });

BLOOPER_STATIC_ASSERT(
    equal(
        [] {
          auto res = false;
          make_void_predicate_matcher(
              [](auto&& o) { return BLOOPER_FORWARD(o) == 1; },
              [&res](auto&&) { res = true; })(int_c<1>);
          return res;
        }(),
        true));

BLOOPER_STATIC_ASSERT(
    equal(
        make_value_matcher(
            1,
            [](auto&&) { return true; })(1),
        std::optional<bool>(true)));

BLOOPER_STATIC_ASSERT(
    equal(
        make_value_matcher(
            2,
            [](auto&&) { return true; })(1),
        std::optional<bool>(std::nullopt)));

BLOOPER_STATIC_ASSERT(
    equal(
        make_concept_matcher(
            concept_c<IntegralConstant>,
            [](auto&&) { return true; })(int_c<1>),
        std::make_optional(true)));

BLOOPER_STATIC_ASSERT(
    equal(
        make_tag_matcher(
            type_c<integral_constant_tag<int>>,
            [](auto&&) { return true; })(int_c<1>),
        std::make_optional(true)));

BLOOPER_STATIC_ASSERT(
    equal(
        make_tag_matcher(
            type_c<tuple_tag>,
            [](auto&&) { return true; })(int_c<1>),
        std::optional<bool>{std::nullopt}));

BLOOPER_STATIC_ASSERT(
    equal(
        make_type_matcher(
            type_c<int_<1>>,
            [](auto&&) { return true; })(int_c<1>),
        std::make_optional(true)));

BLOOPER_STATIC_ASSERT(
    equal(
        make_type_matcher(
            type_c<int_<2>>,
            [](auto&&) { return true; })(int_c<1>),
        std::optional<bool>{std::nullopt}));

[[maybe_unused]] inline constexpr auto make_void_pipe_matcher =
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

[[maybe_unused]] inline constexpr auto make_void_static_matcher =
    ([](auto&& t, auto&& f) {
      return make_void_pipe_matcher(
          [](auto&& o) {
            return static_cast<decltype(origin(BLOOPER_FORWARD(t)))>(
                BLOOPER_FORWARD(o));
          },
          BLOOPER_FORWARD(f));
    });

[[maybe_unused]] inline constexpr auto make_void_dynamic_matcher =
    ([](auto&& t, auto&& f) {
      return make_pipe_matcher(
          [](auto&& o) {
            return dynamic_cast<decltype(origin(BLOOPER_FORWARD(t)))>(
                BLOOPER_FORWARD(o));
          },
          BLOOPER_FORWARD(f));
    });

BLOOPER_STATIC_ASSERT(
    equal(
        make_pipe_matcher(
            [](auto&& o) { return BLOOPER_FORWARD(o) - 1; },
            [](auto&&) { return true; })(int_c<1>),
        std::optional<bool>(std::nullopt)));

BLOOPER_STATIC_ASSERT(
    equal(
        make_static_matcher(
            type_c<int>,
            [](auto&& o) { return BLOOPER_FORWARD(o); })(1.0),
        std::make_optional(1)));

// TODO: dynamic matcher assert?


// Matcher compose

[[maybe_unused]] inline constexpr auto matcher_constexpr_compose =
    ([](auto&& first, auto&& second) -> decltype(auto) {
      return [first = BLOOPER_FORWARD(first),
              second = BLOOPER_FORWARD(second)](auto&& object) {
        constexpr auto first_result_type =
            BLOOPER_TYPEID(first(BLOOPER_FORWARD(object)));

        if constexpr (is_optional(first_result_type))
        {
          if constexpr (is_nothing(origin(first_result_type)))
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

[[maybe_unused]] inline constexpr auto matcher_constexpr_fold =
    ([](auto&& first, auto&&... rest) -> decltype(auto) {
      return fold_left(
          make_tuple(BLOOPER_FORWARD(rest)...),
          BLOOPER_FORWARD(first),
          matcher_constexpr_compose);
    });

BLOOPER_STATIC_ASSERT(
    equal(
        matcher_constexpr_compose(
            make_constexpr_type_matcher(
                type_c<int_<1>>,
                [](auto&&) { return false_c; }),
            make_constexpr_type_matcher(
                type_c<int_<2>>,
                [](auto&&) { return true_c; }))(int_c<1>),
        just(false_c)));

BLOOPER_STATIC_ASSERT(
    equal(
        matcher_constexpr_fold(
            make_constexpr_type_matcher(
                type_c<int_<1>>,
                make_constexpr_argument_matcher(
                    [](decltype(int_c<3>)) { return false_c; })),
            make_constexpr_type_matcher(
                type_c<int_<1>>,
                [](auto&&) { return true_c; }),
            make_constexpr_type_matcher(
                type_c<int_<2>>,
                [](auto&&) { return false_c; }))(int_c<1>),
        just(true_c)));

BLOOPER_STATIC_ASSERT(
    equal(
        matcher_constexpr_fold(
            make_constexpr_concept_matcher(
                concept_c<Searchable>,
                [](auto&&) { return false_c; }),
            make_constexpr_concept_matcher(
                concept_c<IntegralConstant>,
                [](auto&&) { return true_c; }),
            make_constexpr_type_matcher(
                typeid_(int_c<1>),
                [](auto&&) { return false_c; }),
            make_constexpr_type_matcher(
                typeid_(int_c<2>),
                [](auto&&) { return false_c; }))(int_c<1>),
        just(true_c)));

BLOOPER_STATIC_ASSERT(
    equal(
        matcher_constexpr_fold(
            make_constexpr_tag_matcher(
                type_c<optional_tag>,
                [](auto&&) { return false_c; }),
            make_constexpr_tag_matcher(
                type_c<integral_constant_tag<int>>,
                make_constexpr_argument_matcher(
                    [](decltype(int_c<3>)&&) { return false_c; })),
            make_constexpr_tag_matcher(
                type_c<integral_constant_tag<int>>,
                [](auto&&) { return true_c; }),
            make_constexpr_type_matcher(
                typeid_(int_c<2>),
                [](auto&&) { return false_c; }))(int_c<2>),
        just(true_c)));


[[maybe_unused]] inline constexpr auto matcher_compose =
    ([](auto&& first, auto&& second) -> decltype(auto) {
      return [first = BLOOPER_FORWARD(first),
              second = BLOOPER_FORWARD(second)](auto&& object) {
        static_assert(
            equal(
                BLOOPER_TYPEID(first(BLOOPER_FORWARD(object))),
                BLOOPER_TYPEID(second(BLOOPER_FORWARD(object)))),
            "Runtime matcher result types must match. "
            "Be careful when combining different types of matchers.");


        constexpr auto first_result_type =
            BLOOPER_TYPEID(first(BLOOPER_FORWARD(object)));

        if constexpr (is_std_optional(first_result_type))
        {
          auto&& first_result = first(BLOOPER_FORWARD(object));

          if (!BLOOPER_FORWARD(first_result).has_value())
            return second(BLOOPER_FORWARD(object));
          else
            return BLOOPER_FORWARD(first_result);
        }

        return first(BLOOPER_FORWARD(object));
      };
    });

[[maybe_unused]] inline constexpr auto matcher_fold =
    ([](auto&& first, auto&&... rest) -> decltype(auto) {
      return fold_left(
          make_tuple(BLOOPER_FORWARD(rest)...),
          BLOOPER_FORWARD(first),
          matcher_compose);
    });

BLOOPER_STATIC_ASSERT(
    equal(
        matcher_compose(
            make_type_matcher(
                type_c<int_<1>>,
                [](auto&&) { return false; }),
            make_type_matcher(
                type_c<int_<2>>,
                [](auto&&) { return true; }))(int_c<1>),
        std::make_optional(false)));

BLOOPER_STATIC_ASSERT(
    equal(
        matcher_fold(
            make_type_matcher(
                type_c<int_<1>>,
                [](auto&&) { return true; }),
            make_type_matcher(
                type_c<int_<2>>,
                [](auto&&) { return false; }))(int_c<1>),
        std::make_optional(true)));

BLOOPER_STATIC_ASSERT(
    equal(
        matcher_fold(
            make_concept_matcher(
                concept_c<Searchable>,
                [](auto&&) { return false; }),
            make_concept_matcher(
                concept_c<IntegralConstant>,
                [](auto&&) { return true; }),
            make_type_matcher(
                typeid_(int_c<1>),
                [](auto&&) { return false; }),
            make_type_matcher(
                typeid_(int_c<2>),
                [](auto&&) { return false; }))(int_c<1>),
        std::make_optional(true)));

BLOOPER_STATIC_ASSERT(
    equal(
        matcher_fold(
            make_tag_matcher(
                type_c<optional_tag>,
                [](auto&&) { return false; }),
            make_tag_matcher(
                type_c<integral_constant_tag<int>>,
                [](auto&&) { return true; }),
            make_type_matcher(
                typeid_(int_c<2>),
                [](auto&&) { return true; }))(int_c<2>),
        std::make_optional(true)));

BLOOPER_STATIC_ASSERT(
    equal(
        matcher_fold(
            make_value_matcher(
                int_c<2>,
                [](auto&&) { return false; }),
            make_value_matcher(
                3,
                [](auto&&) { return true; }),
            make_value_matcher(
                int_c<3>,
                [](auto&&) { return false; }))(int_c<3>),
        std::make_optional(true)));

BLOOPER_STATIC_ASSERT(
    equal(
        matcher_fold(
            make_type_matcher(
                typeid_(int_c<1>),
                [](auto&&) { return false; }))(int_c<2>),
        std::optional<bool>(std::nullopt)));


[[maybe_unused]] inline constexpr auto matcher_void_compose =
    ([](auto&& first, auto&& second) -> decltype(auto) {
      return [first = BLOOPER_FORWARD(first),
              second = BLOOPER_FORWARD(second)](auto&& object) {
        first(BLOOPER_FORWARD(object));
        second(BLOOPER_FORWARD(object));
      };
    });

[[maybe_unused]] inline constexpr auto matcher_void_fold =
    ([](auto&& first, auto&&... rest) -> decltype(auto) {
      return [matchers = make_tuple(
                  BLOOPER_FORWARD(first),
                  BLOOPER_FORWARD(rest)...)](auto&& object) {
        for_each(
            matchers,
            [object = BLOOPER_FORWARD(object)](auto&& matcher) {
              matcher(object);
            });
      };
    });

BLOOPER_META_NAMESPACE_END

#endif // BLOOPER_META_HPP
