#ifndef BLOOPER_DEV_VALUE_TREE_COMPONENT_HPP
#define BLOOPER_DEV_VALUE_TREE_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class DevValueTreeComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(DevValueTreeComponent);


  struct Options
  {
  } options;

  explicit DevValueTreeComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});


  [[maybe_unused]] void resized() override;


 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DevValueTreeComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_DEV_VALUE_TREE_COMPONENT_HPP
