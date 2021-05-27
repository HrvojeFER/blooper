#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

BodyMenuBarComponent::BodyMenuBarComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          std::move(state)),
      options(std::move(options))
{
}


void BodyMenuBarComponent::resized()
{
}

BLOOPER_NAMESPACE_END
