#include <blooper/body/panels/PanelsComponent.hpp>

#include <blooper/internal/abstract/id.hpp>
#include <blooper/internal/abstract/const.hpp>
#include <blooper/internal/abstract/meta.hpp>
#include <blooper/internal/ext/value_tree.hpp>
#include <blooper/internal/ext/component.hpp>
#include <blooper/internal/utils/ContextCommands.hpp>

#include <blooper/body/panels/automation/AutomationPanelComponent.hpp>
#include <blooper/body/panels/browser/BrowserPanelComponent.hpp>
#include <blooper/body/panels/control_surface/ControlSurfacePanelComponent.hpp>
#include <blooper/body/panels/master/MasterPanelComponent.hpp>
#include <blooper/body/panels/project/ProjectPanelComponent.hpp>

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
  this->appearanceSettings =
      this->getContext()
          .getSettings()
          .getOrCreateChildWithName(
              id::appearance,
              nullptr);

  auto undoManager =
      std::addressof(
          this->getContext().getUndoManager());


  ext::referTo(
      this->masterTrackPanelSize,
      this->appearanceSettings,
      id::masterTrackPanelSize,
      undoManager,
      defaultMasterTrackPanelSize);

  ext::referTo(
      this->controlSurfacePanelSize,
      this->appearanceSettings,
      id::controlSurfacePanelSize,
      undoManager,
      defaultControlSurfacePanelSize);

  ext::referTo(
      this->browserPanelSize,
      this->appearanceSettings,
      id::browserPanelSize,
      undoManager,
      defaultBrowserPanelSize);


  this->masterTrackPanelOpen.referTo(
      this->getState(),
      PanelsComponent::masterTrackPanelOpenId,
      undoManager,
      defaultMasterTrackPanelOpen);

  this->controlSurfacePanelOpen.referTo(
      this->getState(),
      PanelsComponent::controlSurfacePanelOpenId,
      undoManager,
      defaultControlSurfacePanelOpen);

  this->browserPanelOpen.referTo(
      this->getState(),
      PanelsComponent::browserPanelOpenId,
      undoManager,
      defaultBrowserPanelOpen);


  ProjectPanelOptions projectPanelOptions{};

  this->projectPanel = std::make_unique<ProjectPanelComponent>(
      this->getContext(),
      this->getState().getOrCreateChildWithName(
          ProjectPanelComponent::stateId,
          nullptr),
      move(projectPanelOptions));


  MasterPanelComponent::Options masterPanelOptions{};

  this->masterTrackPanel = std::make_unique<MasterPanelComponent>(
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

  BrowserPanelComponent::Options browserPanelOptions{};

  this->browserPanel = std::make_unique<BrowserPanelComponent>(
      this->getContext(),
      this->getState().getOrCreateChildWithName(
          BrowserPanelComponent::stateId,
          nullptr),
      move(browserPanelOptions));


  ext::addAndMakeVisible(
      *this,
      *this->projectPanel,
      *this->masterTrackPanel,
      *this->controlSurfacePanel,
      *this->browserPanel);


  this->appearanceSettings.addListener(this);
  this->getContext().registerCommandTarget(this);
}

PanelsComponent::~PanelsComponent()
{
  this->getContext().unregisterCommandTarget(this);
  this->appearanceSettings.removeListener(this);
}


// Component

void PanelsComponent::resized()
{
  auto availableArea = this->getLocalBounds();

  if (this->controlSurfacePanelOpen)
  {
    this->controlSurfacePanel->setBounds(
        availableArea
            .removeFromBottom(
                this->controlSurfacePanelSize)
            .reduced(2));
  }

  if (this->masterTrackPanelOpen)
  {
    this->masterTrackPanel->setBounds(
        availableArea
            .removeFromRight(
                this->masterTrackPanelSize)
            .reduced(2));
  }

  if (this->browserPanelOpen)
  {
    this->browserPanel->setBounds(
        availableArea
            .removeFromLeft(
                this->browserPanelSize)
            .reduced(2));
  }

  this->projectPanel->setBounds(availableArea.reduced(2));
}


// ValueTreeListener

[[maybe_unused]] void PanelsComponent::valueTreePropertyChanged(
    juce::ValueTree&        tree,
    const juce::Identifier& id)
{
  if (tree == this->getState())
  {
    if (id == PanelsComponent::masterTrackPanelOpenId)
    {
      this->masterTrackPanel->setVisible(
          this->masterTrackPanelOpen);
    }
    else if (id == PanelsComponent::controlSurfacePanelOpenId)
    {
      this->controlSurfacePanel->setVisible(
          this->controlSurfacePanelOpen);
    }
    else if (id == PanelsComponent::browserPanelOpenId)
    {
      this->browserPanel->setVisible(
          this->browserPanelOpen);
    }

    this->resized();
  }

  else if (tree == this->appearanceSettings)
  {
    if (id == id::masterTrackPanelSize ||
        id == id::controlSurfacePanelSize ||
        id == id::browserPanelSize)
    {
      this->resized();
    }
  }
}


// CommandTarget

JuceCommandTarget* PanelsComponent::getNextCommandTarget()
{
  return nullptr;
}

void PanelsComponent::getAllCommands(juce::Array<JuceCommandId>& commands)
{
  fillCommands(
      commands,
      CommandId::toggleMasterTrackPanel,
      CommandId::toggleControlSurfacePanel,
      CommandId::toggleBrowserPanel);
}

void PanelsComponent::getCommandInfo(JuceCommandId id, JuceCommandInfo& info)
{
  fillCommandInfo(info, id);

  info.setActive(true);
}

bool PanelsComponent::perform(const JuceCommand& command)
{
  switch (command.commandID)
  {
    case CommandId::toggleMasterTrackPanel:
      toggle(this->masterTrackPanelOpen);
      return true;

    case CommandId::toggleControlSurfacePanel:
      toggle(this->controlSurfacePanelOpen);
      return true;

    case CommandId::toggleBrowserPanel:
      toggle(this->browserPanelOpen);
      return true;

    default:
      return false;
  }
}

BLOOPER_NAMESPACE_END
