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

  menuBar =
      std::make_unique<HeaderMenuBarComponent>(
          context,
          this->getState().getOrCreateChildWithName(
              HeaderMenuBarComponent::stateId,
              nullptr),
          move(menuBarOptions));


  HeaderToolbarComponent::Options toolbarOptions{};

  toolbar =
      std::make_unique<HeaderToolbarComponent>(
          context,
          this->getState().getOrCreateChildWithName(
              HeaderToolbarComponent::stateId,
              nullptr),
          move(toolbarOptions));


  ext::addAndMakeVisible(
      *this,
      *menuBar,
      *toolbar);
}


void HeaderComponent::resized()
{
}

BLOOPER_NAMESPACE_END
