#include <blooper/components/dev/DevTransportComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

DevTransportComponent::DevTransportComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}

DevTransportComponent::~DevTransportComponent() = default;


void DevTransportComponent::resized()
{
}

BLOOPER_NAMESPACE_END
