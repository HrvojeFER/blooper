#include <blooper/components/help/HelpComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

HelpComponent::HelpComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}

HelpComponent::~HelpComponent() = default;


void HelpComponent::resized()
{
}

BLOOPER_NAMESPACE_END
