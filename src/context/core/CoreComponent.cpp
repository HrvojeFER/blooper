#include <blooper/blooper.hpp>


BLOOPER_NAMESPACE_BEGIN

CoreComponent::CoreComponent(CoreContext& context)
    : juce::Component(),
      context(context)
{
    setLookAndFeel(&context.getLookAndFeel());
}

CoreComponent::~CoreComponent() = default;

BLOOPER_NAMESPACE_END
