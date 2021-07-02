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

template<bool Condition, typename T = void>
struct disable_if
{
  using type = T;
};

template<typename T>
struct disable_if<true, T>
{
};

template<bool Condition, typename T = void>
using disable_if_t [[maybe_unused]] = typename disable_if<Condition, T>::type;

BLOOPER_STD_NAMESPACE_END

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


// Types

[[maybe_unused]] inline constexpr auto origin =
    [](auto&& t) -> typename std::remove_reference_t<decltype(t)>::type {};

[[maybe_unused]] inline constexpr auto is_type =
    is_valid(origin);

BLOOPER_STATIC_ASSERT(is_type(type_c<bool>));
BLOOPER_STATIC_ASSERT(and_(is_type(type_c<bool>), is_type(type_c<int>)));


template<typename T>
[[maybe_unused]] inline constexpr type<std::remove_cvref_t<T>> typeid_c{};

namespace detail
{
template<typename T, typename = type_tag>
struct decl;

template<typename T>
using decl_t = typename decl<T>::type;

template<typename T, typename>
struct decl
{
  using type = T;
};

template<typename T>
struct decl<T, typename tag_of<T>::type>
{
  using type = decl_t<typename std::remove_reference_t<T>::type>;
};
} // namespace detail

[[maybe_unused]] inline constexpr auto decl =
    ([](auto&& v) -> type<detail::decl_t<decltype(v)>> {
      return {};
    });

BLOOPER_STATIC_ASSERT(
    traits::is_same(
        decl(1),
        type_c<int&&>));

BLOOPER_STATIC_ASSERT(
    traits::is_same(
        decl(type_c<int&>),
        type_c<int&>));

BLOOPER_STATIC_ASSERT(
    traits::is_same(
        decl(type_c<const int&&>),
        type_c<const int&&>));

BLOOPER_STATIC_ASSERT(
    traits::is_same(
        decl(static_cast<const int&&>(1)),
        type_c<const int&&>));

BLOOPER_STATIC_ASSERT(
    traits::is_same(
        decl(static_cast<const int&>(1)),
        type_c<const int&>));


namespace detail
{
template<typename T, typename = type_tag>
struct decll;

template<typename T>
using decll_t = typename decll<T>::type;

template<typename T, typename>
struct decll
{
  using type = std::remove_reference_t<T>&;
};

template<typename T>
struct decll<T, typename tag_of<T>::type>
{
  using type = decll_t<typename std::remove_reference_t<T>::type>;
};
} // namespace detail

[[maybe_unused]] inline constexpr auto decll =
    ([](auto&& v) -> type<detail::decll_t<decltype(v)>> {
      return {};
    });

BLOOPER_STATIC_ASSERT(
    traits::is_same(
        decll(1),
        type_c<int&>));

BLOOPER_STATIC_ASSERT(
    traits::is_same(
        decll(type_c<int&>),
        type_c<int&>));

BLOOPER_STATIC_ASSERT(
    traits::is_same(
        decll(type_c<const int&&>),
        type_c<const int&>));


namespace detail
{
template<typename T, typename = type_tag>
struct declr;

template<typename T>
using declr_t = typename declr<T>::type;

template<typename T, typename>
struct declr
{
  using type = std::remove_reference_t<T>&&;
};

template<typename T>
struct declr<T, typename tag_of<T>::type>
{
  using type = declr_t<typename std::remove_reference_t<T>::type>;
};
} // namespace detail

[[maybe_unused]] inline constexpr auto declr =
    ([](auto&& v) -> type<detail::declr_t<decltype(v)>> {
      return {};
    });

BLOOPER_STATIC_ASSERT(
    traits::is_same(
        declr(1),
        type_c<int&&>));

BLOOPER_STATIC_ASSERT(
    traits::is_same(
        declr(type_c<int&>),
        type_c<int&&>));

BLOOPER_STATIC_ASSERT(
    traits::is_same(
        declr(type_c<const int&&>),
        type_c<const int&&>));


// Casts

#define BLOOPER_MAKE_CAST(_cast)                             \
  [[maybe_unused]] inline constexpr auto ENV_CAT(_cast, _) = \
      [](auto&& t) {                                         \
        static_assert(                                       \
            decltype(is_type(t)){},                          \
            ENV_STRING(ENV_CAT(_cast, _) requires a type));  \
                                                             \
        return [](auto&& var) {                              \
          return ENV_EVAL(_cast)<decltype(origin(            \
              BLOOPER_FORWARD(t)))>(                         \
              BLOOPER_FORWARD(var));                         \
        };                                                   \
      }

BLOOPER_MAKE_CAST(static_cast);
BLOOPER_MAKE_CAST(dynamic_cast);
BLOOPER_MAKE_CAST(const_cast);
BLOOPER_MAKE_CAST(reinterpret_cast);

#undef BLOOPER_MAKE_CAST


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

[[maybe_unused]] inline constexpr auto nand =
    complement_of(and_);

BLOOPER_STATIC_ASSERT(nand(true, false));
BLOOPER_STATIC_ASSERT(not_(nand(true, true, true)));
BLOOPER_STATIC_ASSERT(nand(false, false, false));

[[maybe_unused]] inline constexpr auto nor =
    complement_of(or_);

BLOOPER_STATIC_ASSERT(nor(false, false));
BLOOPER_STATIC_ASSERT(not_(nor(true, false, false)));
BLOOPER_STATIC_ASSERT(not_(nor(true, true, true)));


[[maybe_unused]] inline constexpr auto satisfies_all =
    demux(and_);

BLOOPER_STATIC_ASSERT(
    satisfies_all(traits::is_pointer, traits::is_const)(
        type_c<int* const>));

BLOOPER_STATIC_ASSERT(
    not_(satisfies_all(traits::is_pointer, traits::is_const)(
        type_c<int*>)));


[[maybe_unused]] inline constexpr auto doesnt_satisfy_all =
    demux(nand);

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
    demux(nor);

BLOOPER_STATIC_ASSERT(
    doesnt_satisfy_any(traits::is_pointer, traits::is_const)(
        type_c<float>));

BLOOPER_STATIC_ASSERT(
    not_(doesnt_satisfy_any(traits::is_pointer, traits::is_const)(
        type_c<float*>)));

BLOOPER_STATIC_ASSERT(
    not_(doesnt_satisfy_any(traits::is_pointer, traits::is_const)(
        type_c<float* const>)));


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
      auto _construct =
          meta::demux(BLOOPER_FORWARD(mediator))(
              meta::make_tuple);

      auto _unpack =
          meta::reverse_partial(
              meta::unpack,
              BLOOPER_FORWARD(callee));

      return meta::demux(_unpack)(_construct);
    });

BLOOPER_STATIC_ASSERT(
    mediate(
        overload(
            [](auto) { return true; },
            [](auto, auto) { return false; }),
        reverse_partial(drop_front, int_c<1>))(1, 2));

BLOOPER_META_NAMESPACE_END


BLOOPER_NAMESPACE_BEGIN

// ignores any maybe unused value warning
template<typename... Ts>
[[maybe_unused]] inline constexpr auto maybeUnused(Ts&&...) noexcept
{
}

// ignores Clang-tidy performance-move-const-arg
template<typename T>
[[maybe_unused, nodiscard]] inline constexpr std::remove_reference_t<T>&&
move(T&& v) noexcept
{
  return static_cast<std::remove_reference_t<T>&&>(v);
}

// the name is useful for adding meaning
template<typename T>
[[maybe_unused]] inline constexpr auto toggle(T& _) noexcept
    -> decltype(auto)
{
  return _ = !_;
}

BLOOPER_NAMESPACE_END

#endif //BLOOPER_META_HPP
