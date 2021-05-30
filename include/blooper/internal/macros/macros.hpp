#ifndef BLOOPER_MACROS_HPP
#define BLOOPER_MACROS_HPP


// TODO: flag to disable
#define BLOOPER_STATIC_ASSERT(...) static_assert(__VA_ARGS__)

#define BLOOPER_ASSERT(...) jassert(__VA_ARGS__)


#define BLOOPER_UNUSED(...) static_cast<void>(__VA_ARGS__)


#define BLOOPER_STATE_ID(_name)                \
  /* NOLINTNEXTLINE(cert-err58-cpp) */         \
  inline static const juce::Identifier stateId \
  {                                            \
    #_name                                     \
  }

#define BLOOPER_ID(_name)                    \
  /* NOLINTNEXTLINE(cert-err58-cpp) */       \
  inline static const juce::Identifier _name \
  {                                          \
    #_name                                   \
  }


#include <blooper/internal/macros/namespaces.hpp>


#endif //BLOOPER_MACROS_HPP
