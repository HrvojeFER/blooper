#ifndef BLOOPER_INFO_COMPONENT_HPP
#define BLOOPER_INFO_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class InfoComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(InfoComponent);


  struct Options
  {
  } options;

  explicit InfoComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});


  [[maybe_unused]] void resized() override;


 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InfoComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_INFO_COMPONENT_HPP
