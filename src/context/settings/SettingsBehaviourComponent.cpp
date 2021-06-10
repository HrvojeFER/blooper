#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

SettingsBehaviourComponent::SettingsBehaviourComponent(
    AbstractCoreContext& context,
    State                state,
    Options              options)
    : CoreComponentBase(
          context,
          move(state)),
      options(move(options))
{
}


// Component

[[maybe_unused]] void SettingsBehaviourComponent::resized()
{
}

BLOOPER_NAMESPACE_END
