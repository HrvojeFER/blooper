#ifndef BLOOPER_FAQ_COMPONENT_HPP
#define BLOOPER_FAQ_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class FAQComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(FAQComponent);


  struct Options
  {
  } options;

  explicit FAQComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});


  [[maybe_unused]] void resized() override;


 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FAQComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_FAQ_COMPONENT_HPP
