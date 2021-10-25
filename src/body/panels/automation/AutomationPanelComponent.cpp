#include <blooper/body/panels/automation/AutomationPanelComponent.hpp>

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

AutomationPanelComponent::~AutomationPanelComponent() = default;


// Component

void AutomationPanelComponent::resized()
{
}

BLOOPER_NAMESPACE_END
