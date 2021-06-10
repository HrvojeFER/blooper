#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

SettingsDevicesComponent::SettingsDevicesComponent(
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

[[maybe_unused]] void SettingsDevicesComponent::resized()
{
}


BLOOPER_NAMESPACE_END
