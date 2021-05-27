#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

HeaderComponent::HeaderComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          std::move(state)),
      options(std::move(options))
{
  HeaderMenuBarComponent::Options menuBarOptions{};

  menuBar =
      std::make_unique<HeaderMenuBarComponent>(
          context,
          state.getOrCreateChildWithName(
              HeaderMenuBarComponent::stateId,
              nullptr),
          std::move(menuBarOptions));


  HeaderToolbarComponent::Options toolbarOptions{};

  toolbar =
      std::make_unique<HeaderToolbarComponent>(
          context,
          state.getOrCreateChildWithName(
              HeaderToolbarComponent::stateId,
              nullptr),
          std::move(toolbarOptions));


  ext::addAndMakeVisible(
      *this,
      *menuBar,
      *toolbar);
}


void HeaderComponent::resized()
{
}

BLOOPER_NAMESPACE_END
