#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

FAQComponent::FAQComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}


void FAQComponent::resized()
{
}

BLOOPER_NAMESPACE_END
