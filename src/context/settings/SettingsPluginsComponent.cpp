#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

SettingsPluginsComponent::SettingsPluginsComponent(
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

[[maybe_unused]] void SettingsPluginsComponent::resized()
{
}

BLOOPER_NAMESPACE_END
