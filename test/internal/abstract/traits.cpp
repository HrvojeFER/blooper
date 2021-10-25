ENV_MSVC_SUPPRESS_PUSH(4459); // hiding global declarations
#include <env/test.hpp>
ENV_MSVC_SUPPRESS_POP;

#include <blooper/internal/abstract/traits.hpp>

BLOOPER_TRAITS_NAMESPACE_BEGIN
ENV_MSVC_SUPPRESS_PUSH(6326); // constant comparison


ENV_TEST_CASE(traits, type_equality)
{
  EXPECT_TRUE(
      not_(type_c<int> ^ is_convertible_to ^ type_c<void>));

  EXPECT_TRUE(
      not_(type_c<void> ^ is_convertible_to ^ type_c<int>));

  EXPECT_TRUE(
      type_c<int> ^ is_statically_convertible_to ^ type_c<char>);

  EXPECT_TRUE(
      not_(type_c<int> ^ is_statically_convertible_to ^ type_c<int*>));
}

ENV_TEST_CASE(traits, functions)
{
  EXPECT_TRUE(
      doesnt_have_result(typeid_([] {})));

  EXPECT_TRUE(
      typeid_([](auto) {}) ^ doesnt_have_result_with ^ type_c<int>);

  EXPECT_TRUE(
      typeid_([](auto) { return true; }) ^ has_result_with ^ type_c<int>);

  EXPECT_TRUE(
      (typeid_([](auto, auto) { return true; }) ^ has_result_with)(
          type_c<int>,
          type_c<char>) );

  EXPECT_TRUE(
      result_of(typeid_([] { return true; })) ^ is ^ type_c<bool>);
}

ENV_TEST_CASE(traits, manipulation)
{
  EXPECT_TRUE(
      dereferenced(type_c<int*>) ^ is ^ type_c<int&>);
}

ENV_TEST_CASE(traits, invokable)
{
  EXPECT_TRUE(
      is_invokable(typeid_([] {})));

  EXPECT_TRUE(
      typeid_([](bool) {}) ^ is_invokable_with ^ type_c<bool>);


  EXPECT_TRUE(
      is_callback(typeid_([] {})));


  EXPECT_TRUE(
      not_(typeid_([]() {}) ^ is_any_consumer));

  EXPECT_TRUE(
      typeid_([](int) {}) ^ is_any_consumer);

  EXPECT_TRUE(
      typeid_([](int) {}) ^ is_consumer_of ^ type_c<int>);


  EXPECT_TRUE(
      not_(is_any_producer(
          typeid_([] {}))));

  EXPECT_TRUE(
      not_(is_any_producer(
          typeid_([](auto) {}))));

  EXPECT_TRUE(
      is_any_producer(
          typeid_([] { return true; })));

  EXPECT_TRUE(
      not_(
          typeid_([] {}) ^
          is_producer_of ^
          type_c<int>));

  EXPECT_TRUE(
      not_(
          typeid_([](auto) { return true; }) ^
          is_producer_of ^
          type_c<bool>));

  EXPECT_TRUE(
      typeid_([]() { return true; }) ^
      is_producer_of ^
      type_c<bool>);


  EXPECT_TRUE(
      not_(is_delegate(
          typeid_([](int) {}),
          type_c<int>,
          type_c<int>)));

  EXPECT_TRUE(
      is_delegate(
          typeid_([](int, char) { return true; }),
          type_c<bool>,
          type_c<int>,
          type_c<char>));

  EXPECT_TRUE(
      not_(is_delegate(
          typeid_([](int) {}),
          type_c<bool>,
          type_c<int>)));
}


ENV_MSVC_SUPPRESS_POP;
BLOOPER_TRAITS_NAMESPACE_END
