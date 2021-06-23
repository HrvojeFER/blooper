#ifndef BLOOPER_META_HPP
#define BLOOPER_META_HPP
#pragma once

#include <blooper/internal/macros/namespaces.hpp>

BLOOPER_META_NAMESPACE_BEGIN

[[maybe_unused]] inline constexpr auto check =
    ([](auto f) {
      return [f](auto... ts) {
        return decltype(is_valid(f)(
            std::declval<typename decltype(ts)::type>()...)){};
      };
    });

[[maybe_unused]] inline constexpr auto attribute =
    ([](auto f) {
      return [f](auto... ts) {
        return decltype(f(
            std::declval<typename decltype(ts)::type>()...)){};
      };
    });


[[maybe_unused]] inline constexpr auto satisfies_all = demux(and_);

[[maybe_unused]] inline constexpr auto satisfies_any = demux(or_);

[[maybe_unused]] inline constexpr auto inherit =
    infix(
        [](auto f, auto b) {
          return [f, b](auto... ts) {
            if constexpr (decltype(b(ts...)){})
              return decltype(f(ts...)){};
            else
              return false_c;
          };
        });

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
