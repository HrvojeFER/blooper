#ifndef BLOOPER_PRELUDE_HPP
#define BLOOPER_PRELUDE_HPP
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

#endif // BLOOPER_PRELUDE_HPP
