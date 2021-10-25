#include <env/test.hpp>

// TODO: suppress this warning with Env
ENV_MSVC_SUPPRESS_PUSH(6326); // constant comparison

ENV_TEST_CASE(blooper, test)
{
  EXPECT_EQ(true, true);
}

ENV_MSVC_SUPPRESS_POP;
