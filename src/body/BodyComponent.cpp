#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

BodyComponent::BodyComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          std::move(state)),
      options(std::move(options))
{
  PanelsComponent::Options panelsOptions{};

  panels =
      std::make_unique<PanelsComponent>(
          context,
          state.getOrCreateChildWithName(
              PanelsComponent::stateId,
              nullptr),
          std::move(panelsOptions));


  HeaderComponent::Options headerOptions{};

  header =
      std::make_unique<HeaderComponent>(
          context,
          state.getOrCreateChildWithName(
              HeaderComponent::stateId,
              nullptr),
          std::move(headerOptions));


  FooterComponent::Options footerOptions{};

  footer =
      std::make_unique<FooterComponent>(
          context,
          state.getOrCreateChildWithName(
              FooterComponent::stateId,
              nullptr),
          std::move(footerOptions));


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
