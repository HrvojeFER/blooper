#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

MasterTrackComponent::MasterTrackComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          std::move(state)),
      options(std::move(options))
{
}


void MasterTrackComponent::resized()
{
}

BLOOPER_NAMESPACE_END
