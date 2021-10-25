#ifndef BLOOPER_ID_MACROS_HPP
#define BLOOPER_ID_MACROS_HPP
#pragma once

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

#endif // BLOOPER_ID_MACROS_HPP
