#ifndef BLOOPER_ASSERTS_HPP
#define BLOOPER_ASSERTS_HPP
#pragma once

// TODO: flag to disable

#define BLOOPER_STATIC_ASSERT(...) static_assert(__VA_ARGS__)

#define BLOOPER_ASSERT(...) jassert(__VA_ARGS__)

#endif //BLOOPER_ASSERTS_HPP
