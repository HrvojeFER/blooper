#ifndef BLOOPER_TRAITS_HPP
#define BLOOPER_TRAITS_HPP

#include "meta.hpp"

BLOOPER_TRAITS_NAMESPACE_BEGIN

[[maybe_unused]] inline constexpr auto dereference =
    attribute([](auto&& v) -> decltype(typeid_(*v)) {});

BLOOPER_TRAITS_NAMESPACE_END

#endif //BLOOPER_TRAITS_HPP
