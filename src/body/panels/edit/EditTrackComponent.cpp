#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

EditTrackComponent::EditTrackComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}


void EditTrackComponent::resized()
{
}

BLOOPER_NAMESPACE_END
