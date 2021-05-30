#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

EditPanelComponent::EditPanelComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}


void EditPanelComponent::resized()
{
}

BLOOPER_NAMESPACE_END
