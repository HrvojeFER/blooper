#ifndef BLOOPER___NAME___COMPONENT_HPP
#define BLOOPER___NAME___COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

struct __name__ComponentOptions
{
};

class __name__Component :
    public ComponentBase
{
 public:
  BLOOPER_STATE_ID(__name__Component);


  explicit __name__Component(
      AbstractContext&         context,
      State                    state,
      __name__ComponentOptions options = {});

  ~__name__Component() override;

  __name__ComponentOptions options;


  // Component

 public:
  void resized() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(__name__Component)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER___NAME___COMPONENT_HPP
