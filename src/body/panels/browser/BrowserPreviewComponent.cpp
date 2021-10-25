#include <blooper/body/panels/browser/BrowserPreviewComponent.hpp>

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

BrowserPreviewComponent::~BrowserPreviewComponent() = default;


// Component

void BrowserPreviewComponent::resized()
{
}

BLOOPER_NAMESPACE_END
