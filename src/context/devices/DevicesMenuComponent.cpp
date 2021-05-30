#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

DevicesMenuComponent::DevicesMenuComponent(
    AbstractCoreContext& context,
    State                state,
    Options              options)
    : CoreComponentBase(context, move(state)),

      options(move(options)),

      selector(
          getContext().getEngine().getDeviceManager().deviceManager,
          0,
          512,
          1,
          512,
          false,
          false,
          true,
          true)
{
}


void DevicesMenuComponent::resized()
{
  auto availableArea = getLocalBounds();

  selector.setBounds(availableArea);
}

BLOOPER_NAMESPACE_END
