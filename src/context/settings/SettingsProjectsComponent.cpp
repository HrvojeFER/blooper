#include <blooper/context/settings/SettingsProjectsComponent.hpp>

#include <blooper/internal/ext/component.hpp>

BLOOPER_NAMESPACE_BEGIN

SettingsProjectsComponent::SettingsProjectsComponent(
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

[[maybe_unused]] void SettingsProjectsComponent::resized()
{
}

BLOOPER_NAMESPACE_END
