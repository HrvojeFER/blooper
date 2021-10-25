#include <blooper/context/settings/SettingsKeymapsComponent.hpp>

#include <blooper/internal/ext/component.hpp>

BLOOPER_NAMESPACE_BEGIN

SettingsKeymapsComponent::SettingsKeymapsComponent(
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

[[maybe_unused]] void SettingsKeymapsComponent::resized()
{
}

BLOOPER_NAMESPACE_END
