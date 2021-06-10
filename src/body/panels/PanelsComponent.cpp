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
  auto& settings = this->getContext().getSettings();
  settings.addListener(this);
  this->getContext().registerCommandTarget(this);


  auto undoManager = std::addressof(this->getContext().getUndoManager());

  this->masterTrackPanelSize.referTo(
      settings,
      id::masterTrackPanelSize,
      undoManager,
      PanelsComponent::defaultMasterTrackPanelSize);

  this->controlSurfacePanelSize.referTo(
      settings,
      id::controlSurfacePanelSize,
      undoManager,
      PanelsComponent::defaultControlSurfacePanelSize);

  this->browserPanelSize.referTo(
      settings,
      id::browserPanelSize,
      undoManager,
      PanelsComponent::defaultBrowserPanelSize);


  this->masterTrackPanelOpen.referTo(
      this->getState(),
      PanelsComponent::masterTrackPanelOpenId,
      undoManager,
      PanelsComponent::defaultMasterTrackPanelOpen);

  this->controlSurfacePanelSize.referTo(
      this->getState(),
      PanelsComponent::controlSurfacePanelOpenId,
      undoManager,
      PanelsComponent::defaultControlSurfacePanelOpen);

  this->browserPanelSize.referTo(
      this->getState(),
      PanelsComponent::browserPanelOpenId,
      undoManager,
      PanelsComponent::defaultBrowserPanelOpen);


  ProjectPanelComponent::Options projectPanelOptions{};

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
      *this->controlSurfacePanel);
}

PanelsComponent::~PanelsComponent()
{
  this->getContext().unregisterCommandTarget(this);
  this->getContext().getSettings().removeListener(this);
}


// Component

void PanelsComponent::resized()
{
  auto availableArea = this->getLocalBounds();

  if (this->controlSurfacePanelOpen)
  {
    this->controlSurfacePanel->setBounds(
        availableArea.removeFromBottom(
            this->controlSurfacePanelSize));
  }

  if (this->masterTrackPanelOpen)
  {
    this->masterTrackPanel->setBounds(
        availableArea.removeFromRight(
            this->masterTrackPanelSize));
  }

  if (this->browserPanelOpen)
  {
    this->browserPanel->setBounds(
        availableArea.removeFromLeft(
            this->browserPanelSize));
  }

  this->projectPanel->setBounds(availableArea);
}


// ValueTreeListener

[[maybe_unused]] void PanelsComponent::valueTreePropertyChanged(
    juce::ValueTree&        tree,
    const juce::Identifier& id)
{
  if (tree == this->getState() &&
      (id == PanelsComponent::masterTrackPanelOpenId ||
       id == PanelsComponent::controlSurfacePanelOpenId ||
       id == PanelsComponent::browserPanelOpenId))
    this->resized();

  if (tree == this->getContext().getSettings() &&
      (id == id::masterTrackPanelSize ||
       id == id::controlSurfacePanelSize ||
       id == id::browserPanelSize))
    this->resized();
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
}

bool PanelsComponent::perform(const JuceCommand& command)
{
  switch (command.commandID)
  {
    case CommandId::toggleMasterTrackPanel:
      this->masterTrackPanelOpen = !this->masterTrackPanelOpen;
      return true;

    case CommandId::toggleControlSurfacePanel:
      this->controlSurfacePanelOpen = !this->controlSurfacePanelOpen;
      return true;

    case CommandId::toggleBrowserPanel:
      this->browserPanelOpen = !this->browserPanelOpen;
      return true;

    default:
      return false;
  }
}

BLOOPER_NAMESPACE_END
