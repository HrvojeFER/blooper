#include <env/test.hpp>

#include <blooper/internal/abstract/matchers.hpp>

BLOOPER_TEST_NAMESPACE_BEGIN
ENV_MSVC_SUPPRESS_PUSH(6326); // constant comparison

class base
{
 public:
  virtual bool is_base() noexcept
  {
    return true;
  }

  virtual bool is_sub() noexcept
  {
    return false;
  }
};

class sub : public base
{
 public:
  bool is_base() noexcept override
  {
    return false;
  }

  bool is_sub() noexcept override
  {
    return true;
  }
} static _sub;

static base* sub_ptr = std::addressof(_sub);


ENV_TEST_CASE(matchers, constexpr_predicate_matchers)
{
  EXPECT_TRUE(
      meta::equal(
          makeConstexprPredicateMatcher(
              [](auto&& o) { return !meta::traits::is_pointer(meta::typeid_(o)); },
              [](auto&&) { return meta::true_c; })(meta::int_c<1>),
          meta::just(meta::true_c)));

  EXPECT_TRUE(
      meta::equal(
          makeConstexprArgumentMatcher(
              [](meta::int_<1>) { return meta::true_c; })(meta::int_c<1>),
          meta::just(meta::true_c)));

  EXPECT_TRUE(
      meta::equal(
          makeConstexprConceptMatcher(
              meta::concept_c<meta::IntegralConstant>,
              [](auto&&) { return meta::true_c; })(meta::int_c<1>),
          meta::just(meta::true_c)));

  EXPECT_TRUE(
      meta::equal(
          makeConstexprTagMatcher(
              meta::type_c<meta::integral_constant_tag<int>>,
              [](auto&&) { return meta::true_c; })(meta::int_c<1>),
          meta::just(meta::true_c)));

  EXPECT_TRUE(
      meta::equal(
          makeConstexprTagMatcher(
              meta::type_c<meta::tuple_tag>,
              [](auto&&) { return meta::true_c; })(meta::int_c<1>),
          meta::nothing));

  EXPECT_TRUE(
      meta::equal(
          makeConstexprTypeMatcher(
              meta::type_c<meta::int_<1>>,
              [](auto&&) { return meta::true_c; })(meta::int_c<1>),
          meta::just(meta::true_c)));

  EXPECT_TRUE(
      meta::equal(
          makeConstexprTypeMatcher(
              meta::type_c<meta::int_<2>>,
              [](auto&&) { return meta::true_c; })(meta::int_c<1>),
          meta::nothing));
}


ENV_TEST_CASE(matchers, predicate_matchers)
{
  EXPECT_TRUE(
      meta::equal(
          makePredicateMatcher(
              [](auto&& o) { return BLOOPER_FORWARD(o) == 1; },
              [](auto&&) { return true; })(meta::int_c<1>),
          std::make_optional(true)));

  EXPECT_TRUE(
      meta::equal(
          makeValueMatcher(
              1,
              [](auto&&) { return true; })(1),
          std::optional<bool>(true)));

  EXPECT_TRUE(
      meta::equal(
          makeValueMatcher(
              2,
              [](auto&&) { return true; })(1),
          std::optional<bool>(std::nullopt)));

  EXPECT_TRUE(
      meta::equal(
          makeConceptMatcher(
              meta::concept_c<meta::IntegralConstant>,
              [](auto&&) { return true; })(meta::int_c<1>),
          std::make_optional(true)));

  EXPECT_TRUE(
      meta::equal(
          makeTagMatcher(
              meta::type_c<meta::integral_constant_tag<int>>,
              [](auto&&) { return true; })(meta::int_c<1>),
          std::make_optional(true)));

  EXPECT_TRUE(
      meta::equal(
          makeTagMatcher(
              meta::type_c<meta::tuple_tag>,
              [](auto&&) { return true; })(meta::int_c<1>),
          std::optional<bool>{std::nullopt}));

  EXPECT_TRUE(
      meta::equal(
          makeTypeMatcher(
              meta::type_c<meta::int_<1>>,
              [](auto&&) { return true; })(meta::int_c<1>),
          std::make_optional(true)));

  EXPECT_TRUE(
      meta::equal(
          makeTypeMatcher(
              meta::type_c<meta::int_<2>>,
              [](auto&&) { return true; })(meta::int_c<1>),
          std::optional<bool>{std::nullopt}));
}

ENV_TEST_CASE(matchers, pipe_matchers)
{
  EXPECT_TRUE(
      meta::equal(
          makePipeMatcher(
              [](auto&& o) { return BLOOPER_FORWARD(o) - 1; },
              [](auto&&) { return true; })(meta::int_c<1>),
          std::optional<bool>(std::nullopt)));

  EXPECT_TRUE(
      meta::equal(
          makeStaticMatcher(
              meta::type_c<int>,
              [](auto&& o) { return BLOOPER_FORWARD(o); })(1.0),
          std::make_optional(1)));

  EXPECT_TRUE(
      meta::equal(
          makeDynamicMatcher(
              meta::type_c<sub*>,
              [](base* p) { return p->is_sub(); })(sub_ptr),
          std::make_optional(true)));
}

ENV_TEST_CASE(matchers, void_predicate_matchers)
{
  EXPECT_TRUE(
      meta::equal(
          [] {
            auto res = false;
            makeVoidPredicateMatcher(
                [](auto&& o) { return BLOOPER_FORWARD(o) == 1; },
                [&res](auto&&) { res = true; })(meta::int_c<1>);
            return res;
          }(),
          true));

  EXPECT_TRUE(
      meta::equal(
          [] {
            auto res = false;
            makeVoidConceptMatcher(
                meta::concept_c<meta::IntegralConstant>,
                [&res](auto&&) { res = true; })(meta::int_c<1>);
            return res;
          }(),
          true));

  EXPECT_TRUE(
      meta::equal(
          [] {
            auto res = false;
            makeVoidTagMatcher(
                meta::type_c<meta::integral_constant_tag<int>>,
                [&res](auto&&) { res = true; })(meta::int_c<1>);
            return res;
          }(),
          true));

  EXPECT_TRUE(
      meta::equal(
          [] {
            auto res = false;
            makeVoidTagMatcher(
                meta::type_c<meta::tuple_tag>,
                [&res](auto&&) { res = true; })(meta::int_c<1>);
            return res;
          }(),
          false));

  EXPECT_TRUE(
      meta::equal(
          [] {
            auto res = false;
            makeVoidTypeMatcher(
                meta::type_c<meta::int_<1>>,
                [&res](auto&&) { res = true; })(meta::int_c<1>);
            return res;
          }(),
          true));

  EXPECT_TRUE(
      meta::equal(
          [] {
            auto res = false;
            makeVoidTypeMatcher(
                meta::type_c<meta::int_<2>>,
                [&res](auto&&) { res = true; })(meta::int_c<1>);
            return res;
          }(),
          false));

  EXPECT_TRUE(
      meta::equal(
          [] {
            auto res = false;
            makeVoidValueMatcher(
                1,
                [&res](auto&&) { res = true; })(1);
            return res;
          }(),
          true));

  EXPECT_TRUE(
      meta::equal(
          [] {
            auto res = false;
            makeVoidValueMatcher(
                2,
                [&res](auto&&) { res = true; })(1);
            return res;
          }(),
          false));
}

ENV_TEST_CASE(matchers, void_pipe_matchers)
{
  EXPECT_TRUE(
      meta::equal(
          [] {
            auto res = false;
            makeVoidPipeMatcher(
                [](auto&& o) { return BLOOPER_FORWARD(o) - 1; },
                [&res](auto&&) { res = true; })(meta::int_c<1>);
            return res;
          }(),
          false));

  EXPECT_TRUE(
      meta::equal(
          [] {
            auto res = 0;
            makeVoidStaticMatcher(
                meta::type_c<int>,
                [&res](auto&& o) { res = BLOOPER_FORWARD(o); })(1.0);
            return res;
          }(),
          1));

  EXPECT_TRUE(
      meta::equal(
          [] {
            auto res = false;
            makeVoidDynamicMatcher(
                meta::type_c<sub*>,
                [&res](sub* p) { res = p->is_sub(); })(sub_ptr);
            return res;
          }(),
          true));
}

ENV_TEST_CASE(matchers, constexpr_function_matchers)
{
  EXPECT_TRUE(
      meta::equal(
          makeConstexprFunctionPredicateMatcher(
              [](auto&& o) { return !meta::traits::is_pointer(meta::typeid_(o)); },
              [] { return meta::true_c; })(meta::int_c<1>)
              .value()(),
          meta::true_c));

  EXPECT_TRUE(
      meta::equal(
          makeConstexprFunctionConceptMatcher(
              meta::concept_c<meta::IntegralConstant>,
              []() { return meta::true_c; })(meta::int_c<1>)
              .value()(),
          meta::true_c));

  EXPECT_TRUE(
      meta::equal(
          makeConstexprFunctionTagMatcher(
              meta::type_c<meta::integral_constant_tag<int>>,
              [] { return meta::true_c; })(meta::int_c<1>)
              .value()(),
          meta::true_c));

  EXPECT_TRUE(
      meta::equal(
          makeConstexprFunctionTagMatcher(
              meta::type_c<meta::tuple_tag>,
              [] { return meta::true_c; })(meta::int_c<1>),
          meta::nothing));

  EXPECT_TRUE(
      meta::equal(
          makeConstexprFunctionTypeMatcher(
              meta::type_c<meta::int_<1>>,
              [] { return meta::true_c; })(meta::int_c<1>)
              .value()(),
          meta::true_c));

  EXPECT_TRUE(
      meta::equal(
          makeConstexprFunctionTypeMatcher(
              meta::type_c<meta::int_<2>>,
              [] { return meta::true_c; })(meta::int_c<1>),
          meta::nothing));
}

ENV_TEST_CASE(matchers, function_matchers)
{
  EXPECT_TRUE(
      meta::equal(
          makeFunctionPredicateMatcher(
              [](auto&& o) { return BLOOPER_FORWARD(o) == 1; },
              [] { return true; })(meta::int_c<1>)
              .value()(),
          true));

  EXPECT_TRUE(
      meta::equal(
          makeFunctionValueMatcher(
              1,
              [] { return true; })(1)
              .value()(),
          true));

  EXPECT_TRUE(
      makeFunctionValueMatcher(
          2,
          [] { return true; })(1) ==
      std::nullopt);

  EXPECT_TRUE(
      meta::equal(
          makeFunctionConceptMatcher(
              meta::concept_c<meta::IntegralConstant>,
              [] { return true; })(meta::int_c<1>)
              .value()(),
          true));

  EXPECT_TRUE(
      meta::equal(
          makeFunctionTagMatcher(
              meta::type_c<meta::integral_constant_tag<int>>,
              [] { return true; })(meta::int_c<1>)
              .value()(),
          true));

  EXPECT_TRUE(
      makeFunctionTagMatcher(
          meta::type_c<meta::tuple_tag>,
          [] { return true; })(meta::int_c<1>) ==
      std::nullopt);

  EXPECT_TRUE(
      meta::equal(
          makeFunctionTypeMatcher(
              meta::type_c<meta::int_<1>>,
              [] { return true; })(meta::int_c<1>)
              .value()(),
          true));

  EXPECT_TRUE(
      makeFunctionTypeMatcher(
          meta::type_c<meta::int_<2>>,
          [](auto&&) { return true; })(meta::int_c<1>) ==
      std::nullopt);
}

ENV_TEST_CASE(matchers, matcher_constexpr_fold)
{
  EXPECT_TRUE(
      meta::equal(
          matcherConstexprCompose(
              makeConstexprTypeMatcher(
                  meta::type_c<meta::int_<1>>,
                  [](auto&&) { return meta::false_c; }),
              makeConstexprTypeMatcher(
                  meta::type_c<meta::int_<2>>,
                  [](auto&&) { return meta::true_c; }))(meta::int_c<1>),
          meta::just(meta::false_c)));

  EXPECT_TRUE(
      meta::equal(
          matcherConstexprFold(
              makeConstexprTypeMatcher(
                  meta::type_c<meta::int_<1>>,
                  makeConstexprArgumentMatcher(
                      [](decltype(meta::int_c<3>)) { return meta::false_c; })),
              makeConstexprTypeMatcher(
                  meta::type_c<meta::int_<1>>,
                  [](auto&&) { return meta::true_c; }),
              makeConstexprTypeMatcher(
                  meta::type_c<meta::int_<2>>,
                  [](auto&&) { return meta::false_c; }))(meta::int_c<1>),
          meta::just(meta::true_c)));

  EXPECT_TRUE(
      meta::equal(
          matcherConstexprFold(
              makeConstexprConceptMatcher(
                  meta::concept_c<meta::Searchable>,
                  [](auto&&) { return meta::false_c; }),
              makeConstexprConceptMatcher(
                  meta::concept_c<meta::IntegralConstant>,
                  [](auto&&) { return meta::true_c; }),
              makeConstexprTypeMatcher(
                  meta::typeid_(meta::int_c<1>),
                  [](auto&&) { return meta::false_c; }),
              makeConstexprTypeMatcher(
                  meta::typeid_(meta::int_c<2>),
                  [](auto&&) { return meta::false_c; }))(meta::int_c<1>),
          meta::just(meta::true_c)));

  EXPECT_TRUE(
      meta::equal(
          matcherConstexprFold(
              makeConstexprTagMatcher(
                  meta::type_c<meta::optional_tag>,
                  [](auto&&) { return meta::false_c; }),
              makeConstexprTagMatcher(
                  meta::type_c<meta::integral_constant_tag<int>>,
                  makeConstexprArgumentMatcher(
                      [](decltype(meta::int_c<3>)&&) { return meta::false_c; })),
              makeConstexprTagMatcher(
                  meta::type_c<meta::integral_constant_tag<int>>,
                  [](auto&&) { return meta::true_c; }),
              makeConstexprTypeMatcher(
                  meta::typeid_(meta::int_c<2>),
                  [](auto&&) { return meta::false_c; }))(meta::int_c<2>),
          meta::just(meta::true_c)));
}

ENV_TEST_CASE(matchers, matcher_fold)
{
  EXPECT_TRUE(
      meta::equal(
          matcherCompose(
              makeTypeMatcher(
                  meta::type_c<meta::int_<1>>,
                  [](auto&&) { return false; }),
              makeTypeMatcher(
                  meta::type_c<meta::int_<2>>,
                  [](auto&&) { return true; }))(meta::int_c<1>),
          std::make_optional(false)));

  EXPECT_TRUE(
      meta::equal(
          matcherFold(
              makeTypeMatcher(
                  meta::type_c<meta::int_<1>>,
                  [](auto&&) { return true; }),
              makeTypeMatcher(
                  meta::type_c<meta::int_<2>>,
                  [](auto&&) { return false; }))(meta::int_c<1>),
          std::make_optional(true)));

  EXPECT_TRUE(
      meta::equal(
          matcherFold(
              makeConceptMatcher(
                  meta::concept_c<meta::Searchable>,
                  [](auto&&) { return false; }),
              makeConceptMatcher(
                  meta::concept_c<meta::IntegralConstant>,
                  [](auto&&) { return true; }),
              makeTypeMatcher(
                  meta::typeid_(meta::int_c<1>),
                  [](auto&&) { return false; }),
              makeTypeMatcher(
                  meta::typeid_(meta::int_c<2>),
                  [](auto&&) { return false; }))(meta::int_c<1>),
          std::make_optional(true)));

  EXPECT_TRUE(
      meta::equal(
          matcherFold(
              makeTagMatcher(
                  meta::type_c<meta::optional_tag>,
                  [](auto&&) { return false; }),
              makeTagMatcher(
                  meta::type_c<meta::integral_constant_tag<int>>,
                  [](auto&&) { return true; }),
              makeTypeMatcher(
                  meta::typeid_(meta::int_c<2>),
                  [](auto&&) { return true; }))(meta::int_c<2>),
          std::make_optional(true)));

  EXPECT_TRUE(
      meta::equal(
          matcherFold(
              makeValueMatcher(
                  meta::int_c<2>,
                  [](auto&&) { return false; }),
              makeValueMatcher(
                  3,
                  [](auto&&) { return true; }),
              makeValueMatcher(
                  meta::int_c<3>,
                  [](auto&&) { return false; }))(meta::int_c<3>),
          std::make_optional(true)));

  EXPECT_TRUE(
      meta::equal(
          matcherFold(
              makeTypeMatcher(
                  meta::typeid_(meta::int_c<1>),
                  [](auto&&) { return false; }))(meta::int_c<2>),
          std::optional<bool>(std::nullopt)));
}

ENV_TEST_CASE(matchers, matcher_void_fold)
{
}

ENV_TEST_CASE(matchers, constexpr_matcher_function_fold)
{
}

ENV_TEST_CASE(matchers, matcher_function_fold)
{
}

ENV_MSVC_SUPPRESS_POP;
BLOOPER_TEST_NAMESPACE_END
