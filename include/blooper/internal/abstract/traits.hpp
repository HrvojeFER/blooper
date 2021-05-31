#ifndef BLOOPER_TRAITS_HPP
#define BLOOPER_TRAITS_HPP

#include "meta.hpp"

BLOOPER_TRAITS_NAMESPACE_BEGIN

[[maybe_unused]] inline constexpr auto dereference =
    attribute([](auto&& v) -> decltype(type_c<decltype(*v)>) {});

[[maybe_unused]] inline constexpr auto return_type =
    attribute(
        [](auto&& f, auto&&... args)
            -> decltype(type_c<decltype(f(
                            std::forward<decltype(args)>(args)...))>) {});

BLOOPER_TRAITS_NAMESPACE_END

#endif //BLOOPER_TRAITS_HPP
