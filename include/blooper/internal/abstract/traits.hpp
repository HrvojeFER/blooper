#ifndef BLOOPER_TRAITS_HPP
#define BLOOPER_TRAITS_HPP
#pragma once

#include <blooper/internal/abstract/meta.hpp>

BLOOPER_TRAITS_NAMESPACE_BEGIN

[[maybe_unused]] inline constexpr auto dereference =
    attribute([](auto&& v) -> type<decltype(*v)> {});

[[maybe_unused]] inline constexpr auto return_type =
    attribute(
        [](auto&& f, auto&&... args)
            -> type<decltype(f(
                std::forward<decltype(args)>(args)...))> {});

[[maybe_unused]] inline constexpr auto is_castable =
    check(
        [](auto&& from, auto&& to)
            -> decltype(static_cast<std::decay_t<decltype(to)>>(
                std::forward<decltype(from)>(from))) {});

BLOOPER_TRAITS_NAMESPACE_END

BLOOPER_NAMESPACE_BEGIN

[[maybe_unused]] inline constexpr auto isInvokable =
    meta::check(
        [](auto&& toCheck, auto&&... args)
            -> decltype(toCheck(
                std::forward<decltype(args)>(args)...)) {});

[[maybe_unused]] inline constexpr auto isCallback =
    meta::check(
        [](auto&& toCheck)
            -> decltype(toCheck()) {});

BLOOPER_NAMESPACE_END

#endif //BLOOPER_TRAITS_HPP
