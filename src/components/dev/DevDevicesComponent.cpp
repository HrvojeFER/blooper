#include <blooper/components/dev/DevDevicesComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

DevDevicesComponent::DevDevicesComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}

DevDevicesComponent::~DevDevicesComponent() = default;


void DevDevicesComponent::resized()
{
}

BLOOPER_NAMESPACE_END
