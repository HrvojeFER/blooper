#ifndef BLOOPER_DEV_PROJECT_COMPONENT_HPP
#define BLOOPER_DEV_PROJECT_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class DevProjectComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(DevProjectComponent);


  struct Options
  {
  } options;

  explicit DevProjectComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});


  [[maybe_unused]] void resized() override;


 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DevProjectComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_DEV_PROJECT_COMPONENT_HPP
