#ifndef BLOOPER_DEV_COMPONENT_HPP
#define BLOOPER_DEV_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class DevComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(DevComponent);


  struct Options
  {
  } options;

  explicit DevComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});


  [[maybe_unused]] void resized() override;


 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DevComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_DEV_COMPONENT_HPP
