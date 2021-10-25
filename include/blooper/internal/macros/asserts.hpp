#ifndef BLOOPER_ASSERTS_HPP
#define BLOOPER_ASSERTS_HPP
#pragma once

// TODO: flags to disable when not debug

#if ENV_CPP >= 17

  #define BLOOPER_STATIC_ASSERT(...) \
    static_assert(__VA_ARGS__)

#else

  #define BLOOPER_STATIC_ASSERT(...) \
    static_assert(__VA_ARGS__, "Blooper static assert failed.")

#endif

#define BLOOPER_ASSERT(...) jassert(__VA_ARGS__)

#define BLOOPER_ASSERT_FALSE jassertfalse

#endif // BLOOPER_ASSERTS_HPP
