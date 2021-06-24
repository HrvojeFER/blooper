#include <blooper/components/dev/DevEngineComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

DevEngineComponent::DevEngineComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}

DevEngineComponent::~DevEngineComponent() = default;


// Component

void DevEngineComponent::resized()
{
}

BLOOPER_NAMESPACE_END
