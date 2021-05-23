#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

SettingsMenuComponent::SettingsMenuComponent(
    AbstractCoreContext& context,
    State                state,
    Options              options)
    : CoreComponentBase(
          context,
          std::move(state)),
      options(std::move(options))
{
}

BLOOPER_NAMESPACE_END
