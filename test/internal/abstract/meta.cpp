#include <env/test.hpp>

#include <blooper/internal/abstract/meta.hpp>

BLOOPER_META_NAMESPACE_BEGIN
ENV_MSVC_SUPPRESS_PUSH(6326); // constant comparison


ENV_TEST_CASE(meta, types)
{
  EXPECT_TRUE(is_type(type_c<bool>));
  EXPECT_TRUE(not_(is_type(1)));
  EXPECT_TRUE(and_(is_type(type_c<bool>), is_type(type_c<int>)));

  EXPECT_TRUE(
      equal(tag_of_(make_tuple()),
            type_c<tuple_tag>));

  EXPECT_TRUE(
      equal(tag_of_(int_c<1>),
            type_c<integral_constant_tag<int>>));
}

ENV_TEST_CASE(meta, concepts)
{
  EXPECT_TRUE(
      is_concept(
          concept_c<Searchable>));

  EXPECT_TRUE(
      not_(is_concept(
          int_c<1>)));

  EXPECT_TRUE(
      models_concept(
          typeid_(make_tuple()),
          concept_c<Searchable>));

  EXPECT_TRUE(
      not_(models_concept(
          typeid_(int_c<1>),
          concept_c<Searchable>)));

  EXPECT_TRUE(
      models_concept(
          type_c<tuple_tag>,
          concept_c<Searchable>));
}

ENV_TEST_CASE(meta, logic)
{
  EXPECT_TRUE(nand_(true, false));
  EXPECT_TRUE(not_(nand_(true, true, true)));
  EXPECT_TRUE(nand_(false, false, false));

  EXPECT_TRUE(nor_(false, false));
  EXPECT_TRUE(not_(nor_(true, false, false)));
  EXPECT_TRUE(not_(nor_(true, true, true)));

  EXPECT_TRUE(xor_(true, false, true));
  EXPECT_TRUE(not_(xor_(true, true, true)));
  EXPECT_TRUE(not_(xor_(false, false, false)));

  EXPECT_TRUE(nxor_(true, true, true));
  EXPECT_TRUE(nxor_(false, false));
  EXPECT_TRUE(not_(nxor_(true, false, true)));
  EXPECT_TRUE(not_(nxor_(false, true, false)));
}

ENV_TEST_CASE(meta, checks)
{
  EXPECT_TRUE(
      check(
          [](auto&& toCheck)
              -> decltype(BLOOPER_FORWARD(toCheck) +
                          BLOOPER_FORWARD(toCheck)) {})(
          type_c<int>));

  EXPECT_TRUE(
      attribute(
          [](auto&& toCheck)
              -> decltype(traits::is_pointer(typeid_(toCheck))) {})(
          type_c<int*>));
}

ENV_TEST_CASE(meta, check_composition)
{
  EXPECT_TRUE(
      satisfies_all(traits::is_pointer, traits::is_const)(
          type_c<int* const>));

  EXPECT_TRUE(
      not_(satisfies_all(traits::is_pointer, traits::is_const)(
          type_c<int*>)));


  EXPECT_TRUE(
      doesnt_satisfy_all(traits::is_pointer, traits::is_const)(
          type_c<float*>));

  EXPECT_TRUE(
      doesnt_satisfy_all(traits::is_pointer, traits::is_const)(
          type_c<float>));

  EXPECT_TRUE(
      not_(doesnt_satisfy_all(traits::is_pointer, traits::is_const)(
          type_c<float* const>)));


  EXPECT_TRUE(
      satisfies_any(traits::is_pointer, traits::is_const)(
          type_c<int*>));

  EXPECT_TRUE(
      not_(satisfies_any(traits::is_pointer, traits::is_const)(
          type_c<int>)));


  EXPECT_TRUE(
      doesnt_satisfy_any(traits::is_pointer, traits::is_const)(
          type_c<float>));

  EXPECT_TRUE(
      not_(doesnt_satisfy_any(traits::is_pointer, traits::is_const)(
          type_c<float*>)));

  EXPECT_TRUE(
      not_(doesnt_satisfy_any(traits::is_pointer, traits::is_const)(
          type_c<float* const>)));


  EXPECT_TRUE(
      satisfies_some(traits::is_pointer, traits::is_const)(
          type_c<float*>));

  EXPECT_TRUE(
      not_(satisfies_some(traits::is_pointer, traits::is_const)(
          type_c<float* const>)));

  EXPECT_TRUE(
      not_(satisfies_some(traits::is_pointer, traits::is_const)(
          type_c<float>)));


  EXPECT_TRUE(
      doesnt_satisfy_some(traits::is_pointer, traits::is_const)(
          type_c<float* const>));

  EXPECT_TRUE(
      doesnt_satisfy_some(traits::is_pointer, traits::is_const)(
          type_c<float>));

  EXPECT_TRUE(
      not_(doesnt_satisfy_some(traits::is_pointer, traits::is_const)(
          type_c<float*>)));
}

ENV_TEST_CASE(meta, after)
{
  EXPECT_TRUE(
      not_((attribute(
                [](auto&& toCheck)
                    -> decltype(traits::is_same(
                        typeid_(toCheck()),
                        type_c<bool>)) {}) ^
            after ^
            check(
                [](auto&& toCheck)
                    -> decltype(toCheck()) {}))(
          type_c<int>) ));

  EXPECT_TRUE(
      (attribute(
           [](auto&& first, auto&& second)
               -> decltype(traits::is_same(
                   typeid_(first + second),
                   type_c<int>)) {}) ^
       after ^
       check(
           [](auto&& first, auto&& second)
               -> decltype(first + second) {}))(
          type_c<int>,
          type_c<char>) );
}

ENV_TEST_CASE(meta, mediate)
{
  EXPECT_TRUE(
      mediate(
          overload(
              [](auto) { return true; },
              [](auto, auto) { return false; }),
          reverse_partial(drop_front, int_c<1>))(1, 2));
}

ENV_TEST_CASE(meta, optional)
{
  EXPECT_TRUE(is_std_optional(typeid_(std::make_optional(1))));
  EXPECT_TRUE(not_(is_std_optional(typeid_(1))));

  EXPECT_TRUE(is_optional(typeid_(just(true_c))));
  EXPECT_TRUE(is_optional(typeid_(nothing)));
  EXPECT_TRUE(not_(is_optional(typeid_(1))));

  EXPECT_TRUE(equal(just_flat(just(true_c)), just(true_c)));
  EXPECT_TRUE(equal(just_flat(true_c), just(true_c)));
}


ENV_MSVC_SUPPRESS_POP;
BLOOPER_META_NAMESPACE_END
