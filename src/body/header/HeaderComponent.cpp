#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

HeaderComponent::HeaderComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
  HeaderMenuBarComponent::Options menuBarOptions{};

  this->menuBar =
      std::make_unique<HeaderMenuBarComponent>(
          this->getContext(),
          this->getState().getOrCreateChildWithName(
              HeaderMenuBarComponent::stateId,
              nullptr),
          move(menuBarOptions));


  HeaderToolbarComponent::Options toolbarOptions{};

  this->toolbar =
      std::make_unique<HeaderToolbarComponent>(
          this->getContext(),
          this->getState().getOrCreateChildWithName(
              HeaderToolbarComponent::stateId,
              nullptr),
          move(toolbarOptions));


  ext::addAndMakeVisible(
      *this,
      *this->menuBar,
      *this->toolbar);
}


void HeaderComponent::resized()
{
  auto availableArea = this->getLocalBounds();

  this->toolbar->setBounds(availableArea);
}

BLOOPER_NAMESPACE_END
