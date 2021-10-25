#include <env/test.hpp>

#include <blooper/internal/abstract/visitors.hpp>

BLOOPER_NAMESPACE_BEGIN
ENV_MSVC_SUPPRESS_PUSH(6326); // constant comparison


ENV_TEST_CASE(visitors, nonstopping)
{
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
}

ENV_TEST_CASE(visitors, stopping)
{
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
}

ENV_TEST_CASE(visitors, visitor)
{
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
}


ENV_MSVC_SUPPRESS_POP;
BLOOPER_NAMESPACE_END
