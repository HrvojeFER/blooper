#include <blooper/body/footer/FooterComponent.hpp>

#include <blooper/internal/utils/BarComponents.hpp>

#include <blooper/components/stats/CommandStatusComponent.hpp>
#include <blooper/components/stats/SelectionStatusComponent.hpp>
#include <blooper/components/stats/UndoStatusComponent.hpp>
#include <blooper/components/stats/CPUStatusComponent.hpp>
#include <blooper/components/stats/TransportStatusComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

FooterComponent::FooterComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
  this->commandStatus = addChild<CommandStatusComponent>(*this);
  this->selectionStatus = addChild<SelectionStatusComponent>(*this);
  this->undoStatus = addChild<UndoStatusComponent>(*this);
  this->cpuStatus = addChild<CPUStatusComponent>(*this);
  this->transportStatus = addChild<TransportStatusComponent>(*this);
}

FooterComponent::~FooterComponent() = default;


// Component

void FooterComponent::resized()
{
  auto availableArea = this->getLocalBounds();

  util::BarBuilderOptions barOptions{};
  barOptions.height = availableArea.getHeight();

  availableArea =
      util::buildBar(
          this->getLookAndFeel(),
          move(availableArea),
          move(barOptions))
          .addComponentsLeft(
              *this->commandStatus,
              *this->selectionStatus,
              *this->undoStatus)
          .addComponentsRight(
              *this->cpuStatus,
              *this->transportStatus)
          .finish();
}

BLOOPER_NAMESPACE_END
