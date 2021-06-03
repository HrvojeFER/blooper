#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

PanelsComponent::PanelsComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
  EditPanelComponent::Options editPanelOptions{};

  this->editPanel = std::make_unique<EditPanelComponent>(
      this->getContext(),
      this->getState().getOrCreateChildWithName(
          EditPanelComponent::stateId,
          nullptr),
      move(editPanelOptions));

  MasterPanelComponent::Options masterPanelOptions{};

  this->masterPanel = std::make_unique<MasterPanelComponent>(
      this->getContext(),
      this->getState().getOrCreateChildWithName(
          MasterPanelComponent::stateId,
          nullptr),
      move(masterPanelOptions));

  ControlSurfacePanelComponent::Options controlSurfacePanelOptions{};

  this->controlSurfacePanel = std::make_unique<ControlSurfacePanelComponent>(
      this->getContext(),
      this->getState().getOrCreateChildWithName(
          ControlSurfacePanelComponent::stateId,
          nullptr),
      move(controlSurfacePanelOptions));


  ext::addAndMakeVisible(
      *this,
      *this->editPanel,
      *this->masterPanel,
      *this->controlSurfacePanel);
}


void PanelsComponent::resized()
{
  auto availableArea = this->getLocalBounds();

  this->controlSurfacePanel->setBounds(
      availableArea.removeFromBottom(
          static_cast<int>(
              availableArea.getHeight() * 0.2)));

  this->masterPanel->setBounds(
      availableArea.removeFromRight(
          static_cast<int>(
              availableArea.getWidth() * 0.2)));

  this->editPanel->setBounds(availableArea);
}

BLOOPER_NAMESPACE_END
