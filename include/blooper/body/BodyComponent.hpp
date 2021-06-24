#ifndef BLOOPER_BODY_COMPONENT_HPP
#define BLOOPER_BODY_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

class BodyComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(BodyComponent);


  struct Options
  {
  } options;

  explicit BodyComponent(
      AbstractContext& context,
      State            state,
      Options          options);

  ~BodyComponent() override;


 private:
  std::unique_ptr<class PanelsComponent> panels;

  std::unique_ptr<class HeaderComponent> header;
  std::unique_ptr<class FooterComponent> footer;


  // Component

 public:
  void resized() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BodyComponent);
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_BODY_COMPONENT_HPP
