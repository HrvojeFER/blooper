#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

BodyWindow::BodyWindow(
    AbstractContext& context,
    State            state,
    JuceString       name,
    Options          options)
    : WindowBase(
          std::move(name),
          context,
          std::move(state)),
      options(std::move(options))
{
  BodyComponent::Options componentOptions{};

  component =
      std::make_unique<BodyComponent>(
          getContext(),
          getState().getOrCreateChildWithName(
              BodyComponent::stateId,
              nullptr),
          std::move(componentOptions));

  this->setContentNonOwned(
      this->component.get(),
      false);


  BodyMenuBarComponent::Options menuBarOptions{};

  this->setMenuBarComponent(
      new BodyMenuBarComponent(
          getContext(),
          getState().getOrCreateChildWithName(
              BodyMenuBarComponent::stateId,
              nullptr),
          std::move(menuBarOptions)));


  setFullScreen(true);
}


void BodyWindow::closeButtonPressed()
{
  this->options.onClose();
}

BLOOPER_NAMESPACE_END
