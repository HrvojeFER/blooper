#include <blooper/blooper.hpp>

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


void DevDevicesComponent::resized()
{
}

BLOOPER_NAMESPACE_END
