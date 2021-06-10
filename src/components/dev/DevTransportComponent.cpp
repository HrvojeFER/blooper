#include <blooper/blooper.hpp>

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


void DevTransportComponent::resized()
{
}

BLOOPER_NAMESPACE_END
