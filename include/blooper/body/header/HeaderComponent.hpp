#ifndef BLOOPER_HEADER_COMPONENT_HPP
#define BLOOPER_HEADER_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

class HeaderComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(HeaderComponent);


  struct Options
  {
  } options;

  explicit HeaderComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});

  ~HeaderComponent() override;


 private:
  std::unique_ptr<class HeaderMenuBarComponent> menuBar;
  std::unique_ptr<class HeaderToolbarComponent> toolbar;


  // Component

 public:
  void resized() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HeaderComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_HEADER_COMPONENT_HPP
