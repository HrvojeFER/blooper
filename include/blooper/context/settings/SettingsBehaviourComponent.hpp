#ifndef BLOOPER_SETTINGS_ENGINE_COMPONENT_HPP
#define BLOOPER_SETTINGS_ENGINE_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class SettingsBehaviourComponent : public CoreComponentBase
{
 public:
  BLOOPER_STATE_ID(SettingsBehaviourComponent);


  struct Options
  {
  } options;

  explicit SettingsBehaviourComponent(
      AbstractCoreContext& context,
      State                state,
      Options              options = {});


  // Component

 public:
  [[maybe_unused]] void resized() override;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsBehaviourComponent);
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_SETTINGS_ENGINE_COMPONENT_HPP
