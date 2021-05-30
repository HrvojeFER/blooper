#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

AutomationPanelComponent::AutomationPanelComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}


void AutomationPanelComponent::resized()
{
}

BLOOPER_NAMESPACE_END
