#ifndef BLOOPER_META_HPP
#define BLOOPER_META_HPP

#include <blooper/internal/macros/macros.hpp>

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
              return f(ts...);
            else
              return false_c;
          };
        });


template<typename... Ts>
[[maybe_unused]] inline constexpr auto maybe_unused(Ts...) noexcept { }

BLOOPER_META_NAMESPACE_END

#endif //BLOOPER_META_HPP
