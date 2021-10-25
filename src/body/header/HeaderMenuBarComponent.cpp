#include <blooper/body/header/HeaderMenuBarComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

HeaderMenuBarComponent::HeaderMenuBarComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}

HeaderMenuBarComponent::~HeaderMenuBarComponent() = default;


// Component

void HeaderMenuBarComponent::resized()
{
}

BLOOPER_NAMESPACE_END
