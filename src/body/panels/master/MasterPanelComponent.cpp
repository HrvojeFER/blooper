#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

MasterPanelComponent::MasterPanelComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          std::move(state)),
      options(std::move(options))
{
}


void MasterPanelComponent::resized()
{
}

BLOOPER_NAMESPACE_END
