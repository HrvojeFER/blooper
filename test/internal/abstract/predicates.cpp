#include <env/test.hpp>

#include <blooper/internal/abstract/predicates.hpp>

BLOOPER_NAMESPACE_BEGIN
ENV_MSVC_SUPPRESS_PUSH(6326); // constant comparison


ENV_TEST_CASE(predicates, checks)
{
  EXPECT_TRUE(
      meta::typeid_([](int) { return true; }) ^
      isPredicateOf ^
      meta::type_c<int>);

  EXPECT_TRUE(
      meta::not_(
          meta::typeid_([](int) { return true; }) ^
          isPredicateOf ^
          meta::type_c<char*>));

  EXPECT_TRUE(
      meta::not_(
          meta::typeid_([] { return true; }) ^
          isPredicateOf ^
          meta::type_c<char*>));

  EXPECT_TRUE(
      meta::not_(
          meta::typeid_([] {}) ^
          isPredicateOf ^
          meta::type_c<char*>));


  EXPECT_TRUE(
      isAnyPredicate(
          meta::typeid_([](auto) { return true; })));

  EXPECT_TRUE(
      isAnyPredicate(
          meta::typeid_([](int) { return true; })));

  EXPECT_TRUE(
      meta::not_(isAnyPredicate(
          meta::typeid_([] { return true; }))));

  EXPECT_TRUE(
      meta::not_(isAnyPredicate(
          meta::typeid_([] {}))));
}

ENV_TEST_CASE(predicates, predicates)
{
}

ENV_TEST_CASE(predicates, pipe_predicates)
{
}


ENV_MSVC_SUPPRESS_POP;
BLOOPER_NAMESPACE_END
