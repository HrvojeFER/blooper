#include <blooper/components/dev/DevProjectComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

DevProjectComponent::DevProjectComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}

DevProjectComponent::~DevProjectComponent() = default;


void DevProjectComponent::resized()
{
}

BLOOPER_NAMESPACE_END
