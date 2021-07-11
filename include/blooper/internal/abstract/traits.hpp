#ifndef BLOOPER_TRAITS_HPP
#define BLOOPER_TRAITS_HPP
#pragma once

#include <blooper/internal/abstract/prelude.hpp>
#include <blooper/internal/abstract/meta.hpp>

BLOOPER_TRAITS_NAMESPACE_BEGIN

// Type equality

[[maybe_unused]] inline constexpr auto is =
    infix(is_same);

[[maybe_unused]] inline constexpr auto is_not =
    infix(complement_of(is_same));


[[maybe_unused]] inline constexpr auto is_convertible_to =
    infix(traits::is_convertible);

[[maybe_unused]] inline constexpr auto is_not_convertible_to =
    infix(complement_of(traits::is_convertible_to));


[[maybe_unused]] inline constexpr auto is_statically_convertible =
    check(
        [](auto&& f, auto&& t)
            -> decltype(static_cast<std::decay_t<decltype(t)>>(
                BLOOPER_FORWARD(f))) {});

[[maybe_unused]] inline constexpr auto is_statically_convertible_to =
    infix(is_statically_convertible);


[[maybe_unused]] inline constexpr auto is_dynamically_convertible =
    check(
        [](auto&& f, auto&& t)
            -> decltype(dynamic_cast<std::decay_t<decltype(t)>>(
                BLOOPER_FORWARD(f))) {});

[[maybe_unused]] inline constexpr auto is_dynamically_convertible_to =
    infix(is_dynamically_convertible);


// Functions

[[maybe_unused]] inline constexpr auto has_result =
    check(
        [](auto&& f, auto&&... a)
            -> decltype(consume(BLOOPER_FORWARD(f)(BLOOPER_FORWARD(a)...))) {});

[[maybe_unused]] inline constexpr auto has_result_with =
    infix(has_result);


[[maybe_unused]] inline constexpr auto doesnt_have_result =
    complement_of(has_result);

[[maybe_unused]] inline constexpr auto doesnt_have_result_with =
    infix(doesnt_have_result);


[[maybe_unused]] inline constexpr auto result_of =
    attribute(
        [](auto&& f, auto&&... a)
            -> type<decltype(BLOOPER_FORWARD(f)(BLOOPER_FORWARD(a)...))> {});


// Manipulation

[[maybe_unused]] inline constexpr auto dereferenced =
    attribute([](auto&& v) -> type<decltype(*v)> {});


// Invokable

[[maybe_unused]] inline constexpr auto is_invokable =
    check(
        [](auto&& f, auto&&... a)
            -> decltype(BLOOPER_FORWARD(f)(BLOOPER_FORWARD(a)...)) {});

[[maybe_unused]] inline constexpr auto is_invokable_with =
    infix(is_invokable);


[[maybe_unused]] inline constexpr auto is_callback =
    check(
        [](auto&& f)
            -> decltype(BLOOPER_FORWARD(f)()) {});


[[maybe_unused]] inline constexpr auto is_consumer =
    attribute_wrap(
        [](auto&& f, auto&& af, auto&&... ar)
            -> decltype((BLOOPER_FORWARD(f) ^ doesnt_have_result_with)(
                BLOOPER_FORWARD(af),
                BLOOPER_FORWARD(ar)...)) {}) ^
    after ^
    check(
        [](auto&& f, auto&& af, auto&&... ar)
            -> decltype(BLOOPER_FORWARD(f)(
                BLOOPER_FORWARD(af),
                BLOOPER_FORWARD(ar)...)) {});

[[maybe_unused]] inline constexpr auto is_consumer_of =
    infix(is_consumer);

[[maybe_unused]] inline constexpr auto is_any_consumer =
    is_consumer_of ^ type_c<anything>;


[[maybe_unused]] inline constexpr auto is_producer =
    attribute(
        [](auto&& f, auto&& r)
            -> decltype(typeid_(BLOOPER_FORWARD(f)()) ^
                        is_convertible_to ^
                        typeid_(r)) {}) ^
    after ^
    check(
        [](auto&& f, auto&&)
            -> decltype(consume(
                BLOOPER_FORWARD(f)())) {});


[[maybe_unused]] inline constexpr auto is_producer_of =
    infix(is_producer);

[[maybe_unused]] inline constexpr auto is_any_producer =
    is_producer_of ^ type_c<anything>;


[[maybe_unused]] inline constexpr auto is_delegate =
    attribute(
        [](auto&& f, auto&& r, auto&&... a)
            -> decltype(typeid_(BLOOPER_FORWARD(f)(BLOOPER_FORWARD(a)...)) ^
                        is_convertible_to ^
                        typeid_(r)) {}) ^
    after ^
    check(
        [](auto&& f, auto&& r, auto&&... a)
            -> decltype(consume(BLOOPER_FORWARD(f)(BLOOPER_FORWARD(a)...))) {});

BLOOPER_TRAITS_NAMESPACE_END

#endif // BLOOPER_TRAITS_HPP
