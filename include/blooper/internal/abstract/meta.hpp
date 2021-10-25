#ifndef BLOOPER_META_HPP
#define BLOOPER_META_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/prelude.hpp>

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

[[maybe_unused]] inline constexpr auto fabricate =
    [](auto...) {};


// Types - don't use these in evaluated contexts!

[[maybe_unused]] inline constexpr auto origin =
    ([](auto&& t)
         -> typename std::remove_reference_t<decltype(t)>::type&& {
    });

[[maybe_unused]] inline constexpr auto source =
    ([](auto&& t)
         -> typename std::remove_reference_t<decltype(t)>::type {
    });

[[maybe_unused]] inline constexpr auto is_type =
    is_valid(origin);


// Tags

[[maybe_unused]] inline constexpr auto tag_of_ =
    ([](auto&& t)
         -> type<tag_of_t<std::remove_reference_t<decltype(t)>>> {
      return {};
    });

template<typename T>
[[maybe_unused]] inline constexpr auto tag_of_c =
    type_c<tag_of_t<std::remove_reference_t<T>>>;

template<typename T>
[[maybe_unused]] inline constexpr auto typeid_c =
    type_c<std::remove_cv_t<std::remove_reference_t<T>>>;


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
    infix([](auto&& t, auto&& c) {
      static_assert(
          decltype(is_concept(c)){},
          "models_concept requires a concept");

      static_assert(
          decltype(is_type(t)){},
          "models_concept requires a type");

      return
          typename std::remove_reference_t<decltype(c)>::
              template apply<decltype(origin(t))>{};
    });


// Logic

[[maybe_unused]] inline constexpr auto complement_of =
    demux(not_);

[[maybe_unused]] inline constexpr auto nand_ =
    complement_of(and_);

[[maybe_unused]] inline constexpr auto nor_ =
    complement_of(or_);

[[maybe_unused]] inline constexpr auto xor_ =
    demux(and_)(or_, nand_);

[[maybe_unused]] inline constexpr auto nxor_ =
    complement_of(xor_);


// Checks

[[maybe_unused]] inline constexpr auto check =
    ([](auto&& f) {
      maybeUnused(BLOOPER_FORWARD(f));

      return [](auto&&... ts) {
        maybeUnused(BLOOPER_FORWARD(ts)...);

        static_assert(
            decltype(and_(is_type(BLOOPER_FORWARD(ts))...)){},
            "check requires types.");

        return decltype(is_valid(
            BLOOPER_FORWARD(f),
            origin(BLOOPER_FORWARD(ts))...)){};
      };
    });

[[maybe_unused]] inline constexpr auto attribute_wrap =
    ([](auto&& f) {
      maybeUnused(BLOOPER_FORWARD(f));

      return [](auto&&... ts) {
        maybeUnused(BLOOPER_FORWARD(ts)...);

        static_assert(
            decltype(and_(is_type(BLOOPER_FORWARD(ts))...)){},
            "attribute_wrap requires types.");

        return decltype(BLOOPER_FORWARD(f)(BLOOPER_FORWARD(ts)...)){};
      };
    });

[[maybe_unused]] inline constexpr auto attribute =
    ([](auto&& f) {
      maybeUnused(BLOOPER_FORWARD(f));

      return [](auto&&... ts) {
        maybeUnused(BLOOPER_FORWARD(ts)...);

        static_assert(
            decltype(and_(is_type(BLOOPER_FORWARD(ts))...)){},
            "attribute requires types.");

        return decltype(BLOOPER_FORWARD(f)(origin(BLOOPER_FORWARD(ts))...)){};
      };
    });


// Check composition

[[maybe_unused]] inline constexpr auto satisfies_all =
    demux(and_);


[[maybe_unused]] inline constexpr auto doesnt_satisfy_all =
    demux(nand_);


[[maybe_unused]] inline constexpr auto satisfies_any =
    demux(or_);


[[maybe_unused]] inline constexpr auto doesnt_satisfy_any =
    demux(nor_);


[[maybe_unused]] inline constexpr auto satisfies_some =
    demux(xor_);


[[maybe_unused]] inline constexpr auto doesnt_satisfy_some =
    demux(nxor_);


// After

[[maybe_unused]] inline constexpr auto after =
    infix(
        [](auto&& f, auto&& b) {
          maybeUnused(BLOOPER_FORWARD(f), BLOOPER_FORWARD(b));

          return [](auto&&... ts) {
            static_assert(
                decltype(and_(is_type(BLOOPER_FORWARD(ts))...)){},
                "after requires types.");

            if constexpr (decltype(BLOOPER_FORWARD(b)(BLOOPER_FORWARD(ts)...)){})
              return decltype(BLOOPER_FORWARD(f)(BLOOPER_FORWARD(ts)...)){};
            else
              return false_c;
          };
        });


// Mediate

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


// Optional

namespace optional_detail
{
struct [[maybe_unused]] is_std_optional_t
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
} // namespace optional_detail

[[maybe_unused]] inline constexpr auto is_std_optional =
    ([](auto&& t) {
      maybeUnused(BLOOPER_FORWARD(t));

      static_assert(
          decltype(is_type(BLOOPER_FORWARD(t))){},
          "is_std_optional requires a type");

      [[maybe_unused]] constexpr optional_detail::is_std_optional_t f{};
      return decltype(f(std::declval<decltype(origin(BLOOPER_FORWARD(t)))>())){};
    });


namespace optional_detail
{
struct [[maybe_unused]] is_optional_t
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

[[maybe_unused]] inline constexpr auto is_optional =
    ([](auto&& t) {
      maybeUnused(BLOOPER_FORWARD(t));

      static_assert(
          decltype(is_type(BLOOPER_FORWARD(t))){},
          "is_std_optional requires a type");

      [[maybe_unused]] constexpr optional_detail::is_optional_t f{};
      return decltype(f(std::declval<decltype(origin(BLOOPER_FORWARD(t)))>())){};
    });


[[maybe_unused]] inline constexpr auto just_flat =
    ([](auto&& o) {
      maybeUnused(BLOOPER_FORWARD(o));

      constexpr auto t = BLOOPER_TYPEID(o);

      if constexpr (is_optional(t) || is_std_optional(t))
        return BLOOPER_FORWARD(o);
      else
        return just(BLOOPER_FORWARD(o));
    });

BLOOPER_META_NAMESPACE_END

#endif // BLOOPER_META_HPP
