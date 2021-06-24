#include <blooper/body/footer/FooterComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

FooterComponent::FooterComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}

FooterComponent::~FooterComponent() = default;


// Component

void FooterComponent::resized()
{
}

BLOOPER_NAMESPACE_END
