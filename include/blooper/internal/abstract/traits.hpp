#ifndef BLOOPER_TRAITS_HPP
#define BLOOPER_TRAITS_HPP
#pragma once

#include <blooper/internal/abstract/meta.hpp>

BLOOPER_TRAITS_NAMESPACE_BEGIN

// Types

[[maybe_unused]] inline constexpr auto is =
    infix(is_same);


// Functions

[[maybe_unused]] inline constexpr auto has_result =
    check(
        [](auto&& f, auto&&... args)
            -> decltype(consume(BLOOPER_FORWARD(f)(
                BLOOPER_FORWARD(args)...))) {});

[[maybe_unused]] inline constexpr auto has_result_with =
    infix(has_result);

[[maybe_unused]] inline constexpr auto doesnt_have_result =
    complement_of(has_result_with);

[[maybe_unused]] inline constexpr auto doesnt_have_result_with =
    infix(doesnt_have_result);

BLOOPER_STATIC_ASSERT(
    doesnt_have_result(typeid_([] {})));

BLOOPER_STATIC_ASSERT(
    typeid_([](auto) {}) ^ doesnt_have_result_with ^ type_c<int>);

BLOOPER_STATIC_ASSERT(
    typeid_([](auto) { return true; }) ^ has_result_with ^ type_c<int>);

BLOOPER_STATIC_ASSERT(
    (typeid_([](auto, auto) { return true; }) ^ has_result_with)(
        type_c<int>,
        type_c<char>) );


[[maybe_unused]] inline constexpr auto result_of =
    attribute(
        [](auto&& f, auto&&... args)
            -> type<decltype(BLOOPER_FORWARD(f)(
                BLOOPER_FORWARD(args)...))> {});

BLOOPER_STATIC_ASSERT(
    result_of(typeid_([] { return true; })) ^ is ^ type_c<bool>);


// Casting

[[maybe_unused]] inline constexpr auto is_statically_castable_into =
    infix(check(
        [](auto&& from, auto&& to)
            -> decltype(static_cast<std::decay_t<decltype(to)>>(
                BLOOPER_FORWARD(from))) {}));

BLOOPER_STATIC_ASSERT(
    type_c<int> ^ is_statically_castable_into ^ type_c<char>);

BLOOPER_STATIC_ASSERT(
    not_(type_c<int> ^ is_statically_castable_into ^ type_c<int*>));


// Manipulation

[[maybe_unused]] inline constexpr auto dereferenced =
    attribute([](auto&& v) -> type<decltype(*v)> {});

BLOOPER_STATIC_ASSERT(
    dereferenced(type_c<int*>) ^ is ^ type_c<int&>);

BLOOPER_TRAITS_NAMESPACE_END

BLOOPER_NAMESPACE_BEGIN

// Invokable

[[maybe_unused]] inline constexpr auto isInvokable =
    meta::check(
        [](auto&& toCheck, auto&&... args)
            -> decltype(BLOOPER_FORWARD(toCheck)(
                BLOOPER_FORWARD(args)...)) {});

[[maybe_unused]] inline constexpr auto isInvokableWith =
    meta::infix(isInvokable);

BLOOPER_STATIC_ASSERT(
    isInvokable(meta::typeid_([] {})));

BLOOPER_STATIC_ASSERT(
    meta::typeid_([](bool) {}) ^ isInvokableWith ^ meta::type_c<bool>);


[[maybe_unused]] inline constexpr auto isCallback =
    meta::check(
        [](auto&& toCheck)
            -> decltype(BLOOPER_FORWARD(toCheck)()) {});

BLOOPER_STATIC_ASSERT(
    isCallback(meta::typeid_([] {})));


[[maybe_unused]] inline constexpr auto isConsumer =
    meta::satisfies_all(
        isInvokable,
        meta::traits::doesnt_have_result);

[[maybe_unused]] inline constexpr auto isConsumerOf =
    meta::infix(isConsumer);

[[maybe_unused]] inline constexpr auto isAnyConsumer =
    isConsumerOf ^ meta::type_c<meta::anything>;

BLOOPER_STATIC_ASSERT(
    meta::not_(meta::typeid_([]() {}) ^ isAnyConsumer));

BLOOPER_STATIC_ASSERT(
    meta::typeid_([](int) {}) ^ isAnyConsumer);

BLOOPER_STATIC_ASSERT(
    meta::typeid_([](int) {}) ^ isConsumerOf ^ meta::type_c<int>);


[[maybe_unused]] inline constexpr auto isProducer =
    meta::compose(
        meta::traits::is_convertible,
        meta::traits::result_of) ^
    meta::after ^
    meta::mediate(
        isCallback,
        meta::drop_back);

[[maybe_unused]] inline constexpr auto isProducerOf =
    meta::infix(isProducer);

[[maybe_unused]] inline constexpr auto isAnyProducer =
    isProducerOf ^ meta::type_c<meta::anything>;

BLOOPER_STATIC_ASSERT(
    meta::not_(isAnyProducer(
        meta::typeid_([] {}))));

BLOOPER_STATIC_ASSERT(
    meta::not_(isAnyProducer(
        meta::typeid_([](auto) {}))));

BLOOPER_STATIC_ASSERT(
    isAnyProducer(
        meta::typeid_([] { return true; })));

BLOOPER_STATIC_ASSERT(
    meta::not_(
        meta::typeid_([] {}) ^
        isProducerOf ^
        meta::type_c<int>));

BLOOPER_STATIC_ASSERT(
    meta::not_(
        meta::typeid_([](auto) { return true; }) ^
        isProducerOf ^
        meta::type_c<bool>));


// TODO: better

[[maybe_unused]] inline constexpr auto isFunction =
    meta::attribute(
        [](auto&& toCheck, auto&& result, auto&&... args)
            -> decltype(meta::traits::is_same(
                meta::typeid_c<decltype(BLOOPER_FORWARD(toCheck)(
                    BLOOPER_FORWARD(args)...))>,
                meta::typeid_(result))) {}) ^
    meta::after ^
    meta::check(
        [](auto&& toCheck, auto&& result, auto&&... args)
            -> decltype(BLOOPER_FORWARD(toCheck)(
                BLOOPER_FORWARD(args)...)) {});

BLOOPER_STATIC_ASSERT(
    isFunction(
        meta::typeid_([](int) { return true; }),
        meta::type_c<bool>,
        meta::type_c<int>));

BLOOPER_STATIC_ASSERT(
    meta::not_(isFunction(
        meta::typeid_([](int) {}),
        meta::type_c<bool>,
        meta::type_c<int>)));


// Predicates

[[maybe_unused]] inline constexpr auto isAnyPredicate =
    meta::reverse_partial(
        isFunction,
        meta::type_c<bool>,
        meta::type_c<meta::anything>);

[[maybe_unused]] inline constexpr auto isPredicate =
    meta::partial(
        meta::flip(isFunction),
        meta::type_c<bool>);

[[maybe_unused]] inline constexpr auto isPredicateOf =
    meta::infix(isPredicate);

BLOOPER_STATIC_ASSERT(
    isAnyPredicate(
        meta::typeid_([](auto) { return true; })));

BLOOPER_STATIC_ASSERT(
    isAnyPredicate(
        meta::typeid_([](int) { return true; })));

BLOOPER_STATIC_ASSERT(
    meta::not_(isAnyPredicate(
        meta::typeid_([] { return true; }))));

BLOOPER_STATIC_ASSERT(
    meta::not_(isAnyPredicate(
        meta::typeid_([] {}))));

BLOOPER_STATIC_ASSERT(
    meta::typeid_([](int) { return true; }) ^
    isPredicateOf ^
    meta::type_c<int>);

BLOOPER_STATIC_ASSERT(
    meta::not_(
        meta::typeid_([](int) { return true; }) ^
        isPredicateOf ^
        meta::type_c<char*>));

BLOOPER_STATIC_ASSERT(
    meta::not_(
        meta::typeid_([] { return true; }) ^
        isPredicateOf ^
        meta::type_c<char*>));

BLOOPER_STATIC_ASSERT(
    meta::not_(
        meta::typeid_([] {}) ^
        isPredicateOf ^
        meta::type_c<char*>));


// Visitors

enum class VisitDepth
{
  deep,
  shallow,
};

[[maybe_unused]] inline constexpr auto isAnyStoppingVisitor =
    isAnyPredicate;

[[maybe_unused]] inline constexpr auto isStoppingVisitor =
    isPredicate;

[[maybe_unused]] inline constexpr auto isStoppingVisitorOf =
    isPredicateOf;

BLOOPER_STATIC_ASSERT(
    isAnyStoppingVisitor(
        meta::typeid_([](auto) { return true; })));

BLOOPER_STATIC_ASSERT(
    isAnyStoppingVisitor(
        meta::typeid_([](int) { return true; })));

BLOOPER_STATIC_ASSERT(
    meta::not_(isAnyStoppingVisitor(
        meta::typeid_([] { return true; }))));

BLOOPER_STATIC_ASSERT(
    meta::not_(isAnyStoppingVisitor(
        meta::typeid_([] {}))));

BLOOPER_STATIC_ASSERT(
    meta::typeid_([](int) { return true; }) ^
    isStoppingVisitorOf ^
    meta::type_c<int>);

BLOOPER_STATIC_ASSERT(
    meta::not_(
        meta::typeid_([](int) { return true; }) ^
        isStoppingVisitorOf ^
        meta::type_c<char*>));

BLOOPER_STATIC_ASSERT(
    meta::not_(
        meta::typeid_([] { return true; }) ^
        isStoppingVisitorOf ^
        meta::type_c<char*>));

BLOOPER_STATIC_ASSERT(
    meta::not_(
        meta::typeid_([] {}) ^
        isStoppingVisitorOf ^
        meta::type_c<char*>));


[[maybe_unused]] inline constexpr auto isNonStoppingVisitor =
    meta::reverse_partial(isConsumer);

[[maybe_unused]] inline constexpr auto isNonStoppingVisitorOf =
    meta::infix(isNonStoppingVisitor);

BLOOPER_STATIC_ASSERT(
    meta::not_(
        meta::typeid_([](int) { return true; }) ^
        isNonStoppingVisitorOf ^
        meta::type_c<int>));

BLOOPER_STATIC_ASSERT(
    meta::not_(
        meta::typeid_([](int) { return true; }) ^
        isNonStoppingVisitorOf ^
        meta::type_c<char*>));

BLOOPER_STATIC_ASSERT(
    meta::not_(
        meta::typeid_([] { return true; }) ^
        isNonStoppingVisitorOf ^
        meta::type_c<char*>));

BLOOPER_STATIC_ASSERT(
    meta::not_(
        meta::typeid_([] {}) ^
        isNonStoppingVisitorOf ^
        meta::type_c<char*>));


[[maybe_unused]] inline constexpr auto isVisitor =
    meta::satisfies_any(
        isStoppingVisitor,
        isNonStoppingVisitor);

[[maybe_unused]] inline constexpr auto isVisitorOf =
    meta::infix(isVisitor);

BLOOPER_STATIC_ASSERT(
    meta::typeid_([](auto) { return true; }) ^
    isVisitorOf ^
    meta::type_c<int>);

BLOOPER_STATIC_ASSERT(
    meta::not_(
        meta::typeid_([](int) { return true; }) ^
        isVisitorOf ^
        meta::type_c<char*>));

BLOOPER_STATIC_ASSERT(
    meta::not_(
        meta::typeid_([] { return true; }) ^
        isVisitorOf ^
        meta::type_c<char*>));

BLOOPER_STATIC_ASSERT(
    meta::not_(
        meta::typeid_([] {}) ^
        isVisitorOf ^
        meta::type_c<char*>));

BLOOPER_STATIC_ASSERT(
    meta::typeid_([](int) { return true; }) ^
    isVisitorOf ^
    meta::type_c<int>);

BLOOPER_STATIC_ASSERT(
    meta::not_(
        meta::typeid_([](int) { return true; }) ^
        isVisitorOf ^
        meta::type_c<char*>));

BLOOPER_STATIC_ASSERT(
    meta::not_(
        meta::typeid_([] { return true; }) ^
        isVisitorOf ^
        meta::type_c<char*>));

BLOOPER_STATIC_ASSERT(
    meta::not_(
        meta::typeid_([] {}) ^
        isVisitorOf ^
        meta::type_c<char*>));


[[maybe_unused]] inline constexpr auto callVisitor =
    ([](auto&& visitor, auto&& item) {
      static_assert(
          meta::typeid_(visitor) ^ isVisitorOf ^ meta::typeid_(item),
          "callVisitor should receive a Visitor of the passed item");

      if constexpr (isAnyStoppingVisitor(meta::typeid_(visitor)))
      {
        return BLOOPER_FORWARD(visitor)(BLOOPER_FORWARD(item));
      }
      else
      {
        // formatting...
        BLOOPER_FORWARD(visitor)
        (BLOOPER_FORWARD(item));
        return true;
      }
    });

BLOOPER_NAMESPACE_END

#endif //BLOOPER_TRAITS_HPP
