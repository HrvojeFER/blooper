#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

AutomationPanelComponent::AutomationPanelComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          std::move(state)),
      options(std::move(options))
{
}


void AutomationPanelComponent::resized()
{
}

BLOOPER_NAMESPACE_END
