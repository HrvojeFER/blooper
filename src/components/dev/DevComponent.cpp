#include <blooper/components/dev/DevComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

DevComponent::DevComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}

DevComponent::~DevComponent() = default;


void DevComponent::resized()
{
}

BLOOPER_NAMESPACE_END
