#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

PluginsMenuComponent::PluginsMenuComponent(
    AbstractCoreContext& context,
    State                state,
    Options              options)
    : CoreComponentBase(
          context,
          move(state)),
      options(move(options))
{
}

BLOOPER_NAMESPACE_END
