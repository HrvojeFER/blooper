#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

ProjectContentComponent::ProjectContentComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options)),

      updateTracks(false)
{
  auto undoManager = std::addressof(this->getContext().getUndoManager());
  this->trackSize.referTo(
      this->getContext().getSettings(),
      id::trackSize,
      undoManager,
      ProjectContentComponent::defaultTrackSize);

  this->buildTracks();

  this->getContext().getSettings().addListener(this);
  this->getContext().getEditManager().addListener(this);
  this->getContext().registerCommandTarget(this);
}

ProjectContentComponent::~ProjectContentComponent()
{
  this->getContext().unregisterCommandTarget(this);
  this->getContext().getEditManager().removeListener(this);
  this->getContext().getSettings().removeListener(this);
}


[[maybe_unused]] bool ProjectContentComponent::isValidTrackIndex(int index)
{
  return index >= 0 && index < this->trackComponents.size();
}

void ProjectContentComponent::resizeTracks()
{
  auto newBounds = this->getBounds();
  newBounds.setWidth(this->trackSize * this->trackComponents.size());
  this->setBounds(newBounds);
}

[[maybe_unused]] void ProjectContentComponent::buildTracks()
{
  for (int i = 0; i < this->getChildren().size(); ++i)
    if (dynamic_cast<EditTrackComponent*>(this->getChildComponent(i)))
      this->removeChildComponent(i);

  this->trackComponents.clear();

  this->getContext().getEditManager().visit(
      [this](EditTrackRef track) {
        EditTrackComponent::Options componentOptions{};

        auto trackComponent =
            new EditTrackComponent(
                this->getContext(),
                this->getState().getOrCreateChildWithName(
                    EditTrackComponent::stateId,
                    nullptr),
                move(track),
                move(componentOptions));

        this->trackComponents.add(trackComponent);
      });

  for (auto trackComponent : this->trackComponents)
    ext::addAndMakeVisible(*this, *trackComponent);

  this->resizeTracks();
}


// Component

void ProjectContentComponent::resized()
{
  auto availableArea = this->getLocalBounds();
  auto trackWidth = availableArea.getWidth() / this->trackComponents.size();

  for (auto component : this->trackComponents)
    component->setBounds(availableArea.removeFromLeft(trackWidth));
}


// ValueTreeListener

void ProjectContentComponent::valueTreePropertyChanged(
    juce::ValueTree&        tree,
    const juce::Identifier& id)
{
  if (tree == this->getContext().getSettings())
  {
    if (id == id::trackSize)
    {
      this->resizeTracks();
    }
  }
}

void ProjectContentComponent::valueTreeChildAdded(
    juce::ValueTree& tree,
    juce::ValueTree& child)
{
  if (tree == this->getContext().getProjectState())
  {
    if (child.hasType(id::edit))
    {
      this->markAndUpdate(this->updateTracks);
    }
  }
}

void ProjectContentComponent::valueTreeChildRemoved(
    juce::ValueTree& tree,
    juce::ValueTree& child,
    int)
{
  if (tree == this->getContext().getEditManager().getState())
  {
    if (child.hasType(id::edit))
    {
      this->markAndUpdate(this->updateTracks);
    }
  }
}

void ProjectContentComponent::valueTreeChildOrderChanged(
    juce::ValueTree& tree,
    int              childAIndex,
    int              childBIndex)
{
  if (tree == this->getContext().getEditManager().getState())
  {
    if (tree.getChild(childAIndex).hasType(id::edit) &&
        tree.getChild(childBIndex).hasType(id::edit))
    {
      this->markAndUpdate(this->updateTracks);
    }
  }
}


// FlaggedAsyncUpdater

void ProjectContentComponent::handleAsyncUpdate()
{
  if (util::FlaggedAsyncUpdater::compareAndReset(this->updateTracks))
    this->buildTracks();
}


// ApplicationCommandTarget

juce::ApplicationCommandTarget* ProjectContentComponent::getNextCommandTarget()
{
  return nullptr;
}

void ProjectContentComponent::getAllCommands(
    juce::Array<juce::CommandID>& commands)
{
  fillCommands(
      commands,
      CommandId::selectAll);
}

void ProjectContentComponent::getCommandInfo(
    juce::CommandID               commandID,
    juce::ApplicationCommandInfo& result)
{
  fillCommandInfo(
      result,
      commandID);
}

bool ProjectContentComponent::perform(
    const juce::ApplicationCommandTarget::InvocationInfo& info)
{
  if (info.commandID != CommandId::selectAll) return false;

  if (auto selectionManager =
          this->getContext()
              .getEngine()
              .getUIBehaviour()
              .getCurrentlyFocusedSelectionManager())
  {
    selectionManager->deselectAll();
    this->getContext().getEditManager().visit(
        [selectionManager](EditTrack* track) {
          selectionManager->select(track, true);
        });
  }

  return true;
}

BLOOPER_NAMESPACE_END
