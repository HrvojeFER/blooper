#include <blooper/components/dev/DevEditComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

DevEditComponent::DevEditComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}

DevEditComponent::~DevEditComponent() = default;


// Component

void DevEditComponent::resized()
{
}

BLOOPER_NAMESPACE_END
