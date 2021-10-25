#ifndef BLOOPER___NAME___WINDOW_HPP
#define BLOOPER___NAME___WINDOW_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

struct __name__WindowOptions
{
};

class __name__Window :
    public WindowBase
{
 public:
  BLOOPER_STATE_ID(__name__Window);


  explicit __name__Window(
      AbstractContext&      context,
      State                 state,
      __name__WindowOptions options = {});

  ~__name__Window() override;

  __name__WindowOptions options;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(__name__Window)
};


// Show

__name__Window* show__name__(
    AbstractContext&      context,
    __name__WindowOptions options = {});

BLOOPER_NAMESPACE_END

#endif // BLOOPER___NAME___WINDOW_HPP
