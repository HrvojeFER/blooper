#ifndef BLOOPER_TOOLBAR_COMPONENT_HPP
#define BLOOPER_TOOLBAR_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class HeaderToolbarComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(HeaderToolbarComponent);


  struct Options
  {
  } options;

  explicit HeaderToolbarComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});


  void resized() override;


 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HeaderToolbarComponent)
};

BLOOPER_NAMESPACE_END

#endif //BLOOPER_TOOLBAR_COMPONENT_HPP
