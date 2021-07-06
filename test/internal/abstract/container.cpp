ENV_MSVC_SUPPRESS_PUSH(4459); // hiding global declarations
#include <env/test.hpp>
ENV_MSVC_SUPPRESS_POP;

#include <blooper/internal/abstract/container.hpp>

BLOOPER_NAMESPACE_BEGIN
ENV_MSVC_SUPPRESS_PUSH(6326); // constant comparison


ENV_TEST_CASE(container, container)
{
}


ENV_MSVC_SUPPRESS_POP;
BLOOPER_NAMESPACE_END
