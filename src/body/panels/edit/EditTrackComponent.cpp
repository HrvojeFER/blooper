#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

EditTrackComponent::EditTrackComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          std::move(state)),
      options(std::move(options))
{
}


void EditTrackComponent::resized()
{
}

BLOOPER_NAMESPACE_END
