#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

BrowserPreviewComponent::BrowserPreviewComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}


void BrowserPreviewComponent::resized()
{
}

BLOOPER_NAMESPACE_END
