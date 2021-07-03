#ifndef BLOOPER_META_HPP
#define BLOOPER_META_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>

BLOOPER_STD_NAMESPACE_BEGIN

#if ENV_CPP < 20

template<typename T>
struct remove_cvref
{
  using type = std::remove_reference_t<std::remove_cv_t<T>>;
};

template<typename T>
using remove_cvref_t [[maybe_unused]] = typename remove_cvref<T>::type;

#endif // ENV_CPP < 20

BLOOPER_STD_NAMESPACE_END

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

  return (b1 && b2) || !b1;
}

BLOOPER_NAMESPACE_END

BLOOPER_META_NAMESPACE_BEGIN

// Special

struct anything
{
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
    [](auto&& t) -> typename std::remove_reference_t<decltype(t)>::type {};

[[maybe_unused]] inline constexpr auto is_type =
    is_valid(origin);

BLOOPER_STATIC_ASSERT(is_type(type_c<bool>));
BLOOPER_STATIC_ASSERT(not_(is_type(1)));
BLOOPER_STATIC_ASSERT(and_(is_type(type_c<bool>), is_type(type_c<int>)));

template<typename T>
[[maybe_unused]] inline constexpr auto typeid_c =
    type_c<std::remove_const_t<T>>;


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
          meta::demux(BLOOPER_FORWARD(mediator))(
              meta::make_tuple);

      auto unpack =
          meta::reverse_partial(
              meta::unpack,
              BLOOPER_FORWARD(callee));

      return meta::demux(unpack)(construct);
    });

BLOOPER_STATIC_ASSERT(
    mediate(
        overload(
            [](auto) { return true; },
            [](auto, auto) { return false; }),
        reverse_partial(drop_front, int_c<1>))(1, 2));

BLOOPER_META_NAMESPACE_END

#endif //BLOOPER_META_HPP
