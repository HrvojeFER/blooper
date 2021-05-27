#ifndef BLOOPER_FOOTER_COMPONENT_HPP
#define BLOOPER_FOOTER_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class FooterComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(FooterComponent);


  struct Options
  {
  } options;

  explicit FooterComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});


  void resized() override;


 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FooterComponent)
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_FOOTER_COMPONENT_HPP
