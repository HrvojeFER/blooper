#include <blooper/context/core/CoreComponent.hpp>


BLOOPER_NAMESPACE_BEGIN

CoreComponent::CoreComponent(CoreContext& context)
    : context(context)
{
}

CoreComponent::~CoreComponent() = default;

BLOOPER_NAMESPACE_END
