#include <blooper/components/dev/DevValueTreeComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

DevValueTreeComponent::DevValueTreeComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}

DevValueTreeComponent::~DevValueTreeComponent() = default;


void DevValueTreeComponent::resized()
{
}

BLOOPER_NAMESPACE_END
