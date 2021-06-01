#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

BodyComponent::BodyComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
  PanelsComponent::Options panelsOptions{};

  panels =
      std::make_unique<PanelsComponent>(
          context,
          this->getState().getOrCreateChildWithName(
              PanelsComponent::stateId,
              nullptr),
          move(panelsOptions));


  HeaderComponent::Options headerOptions{};

  header =
      std::make_unique<HeaderComponent>(
          context,
          this->getState().getOrCreateChildWithName(
              HeaderComponent::stateId,
              nullptr),
          move(headerOptions));


  FooterComponent::Options footerOptions{};

  footer =
      std::make_unique<FooterComponent>(
          context,
          this->getState().getOrCreateChildWithName(
              FooterComponent::stateId,
              nullptr),
          move(footerOptions));


  ext::addAndMakeVisible(
      *this,
      *panels,
      *header,
      *footer);
}


void BodyComponent::resized()
{
  auto       availableArea = getLocalBounds();
  const auto bodyHeight = availableArea.getHeight();

  header->setBounds(
      availableArea.removeFromTop(
          int(bodyHeight * 0.1)));
  footer->setBounds(
      availableArea.removeFromBottom(
          int(bodyHeight * 0.05)));

  panels->setBounds(availableArea);
}

BLOOPER_NAMESPACE_END
