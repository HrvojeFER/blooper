#include <blooper/body/panels/project/EditContentComponent.hpp>

#include <blooper/internal/abstract/id.hpp>
#include <blooper/internal/abstract/const.hpp>
#include <blooper/internal/ext/value_tree.hpp>
#include <blooper/internal/ext/component.hpp>
#include <blooper/internal/ext/edit.hpp>
#include <blooper/internal/utils/gui.hpp>
#include <blooper/internal/utils/ContextCommands.hpp>

#include <blooper/context/behaviour/AssetManager.hpp>
#include <blooper/context/behaviour/EditManager.hpp>

#include <blooper/components/tracks/TrackComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

EditContentComponent::EditContentComponent(
    AbstractContext&   context,
    State              state,
    JuceEditRef        edit,
    EditContentOptions options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options)),

      edit(std::move(edit)),

      trackUpdate(false)
{
  auto undoManager = this->getContext().getUndoManagerPtr();


  this->appearanceSettings =
      this->getContext()
          .getSettings()
          .getOrCreateChildWithName(
              id::appearance,
              nullptr);

  ext::referTo(
      this->trackSize,
      this->appearanceSettings,
      id::trackSize,
      undoManager,
      defaultTrackSize);


  this->updateTracks();

  this->getContext().registerCommandTarget(this);
  this->appearanceSettings.addListener(this);
  this->edit->state.addListener(this);
}

EditContentComponent::~EditContentComponent()
{
  this->edit->state.removeListener(this);
  this->appearanceSettings.removeListener(this);
  this->getContext().unregisterCommandTarget(this);
}


[[maybe_unused]] bool EditContentComponent::isValidTrackIndex(int index)
{
  return index >= 0 && index < this->trackComponents.size();
}

void EditContentComponent::resizeTracks()
{
  ext::setWidth(
      *this,
      this->trackSize * this->trackComponents.size() +
          // padding in between tracks
          padding * 2 * (this->trackComponents.size() - 1) +
          // padding around all tracks
          padding * 2);
}

[[maybe_unused]] void EditContentComponent::updateTracks()
{
  for (int i = 0; i < this->getChildren().size(); ++i)
    if (dynamic_cast<TrackComponent*>(this->getChildComponent(i)))
      this->removeChildComponent(i);

  this->trackComponents.clear();


  ext::visit<VisitDepth::shallow>(
      *this->edit,
      [this](JuceTrack& track) {
        if (!track.isAudioTrack()) return;

        TrackComponent::Options componentOptions{};

        auto trackChild =
            this->getState()
                .getChildWithProperty(
                    te::IDs::uid,
                    track.itemID.toVar());

        if (!trackChild.isValid())
        {
          trackChild =
              this->getState()
                  .getOrCreateChildWithName(
                      TrackComponent::stateId,
                      nullptr);

          trackChild.setProperty(
              te::IDs::uid,
              track.itemID.toVar(),
              nullptr);
        }

        auto trackComponent =
            new TrackComponent(
                this->getContext(),
                move(trackChild),
                std::addressof(track),
                move(componentOptions));

        this->trackComponents.add(trackComponent);
      });

  for (auto trackComponent : this->trackComponents)
    ext::addAndMakeVisible(*this, *trackComponent);


  this->resizeTracks();
}


// Component

void EditContentComponent::resized()
{
  const auto numTrackComponents = this->trackComponents.size();
  if (numTrackComponents <= 0) return;

  auto availableArea = util::pad(this->getLocalBounds());
  auto trackWidth = availableArea.getWidth() / numTrackComponents;

  for (auto component : this->trackComponents)
  {
    component->setBounds(
        util::pad(
            availableArea.removeFromLeft(trackWidth),
            2));
  }
}


// ValueTreeListener

void EditContentComponent::valueTreePropertyChanged(
    juce::ValueTree&        tree,
    const juce::Identifier& id)
{
  if (tree == this->appearanceSettings)
  {
    if (id == id::trackSize)
    {
      this->resizeTracks();
    }
  }
}

void EditContentComponent::valueTreeChildAdded(
    juce::ValueTree& tree,
    juce::ValueTree& child)
{
  if (tree == this->edit->state)
  {
    if (child.hasType(te::IDs::TRACK))
    {
      this->markAndUpdate(this->trackUpdate);
    }
  }
}

void EditContentComponent::valueTreeChildRemoved(
    juce::ValueTree& tree,
    juce::ValueTree& child,
    int)
{
  if (tree == this->edit->state)
  {
    if (child.hasType(te::IDs::TRACK))
    {
      this->markAndUpdate(this->trackUpdate);
    }
  }
}

void EditContentComponent::valueTreeChildOrderChanged(
    juce::ValueTree& tree,
    int              childAIndex,
    int              childBIndex)
{
  if (tree == this->edit->state)
  {
    if (tree.getChild(childAIndex).hasType(te::IDs::TRACK) ||
        tree.getChild(childBIndex).hasType(te::IDs::TRACK))
    {
      this->markAndUpdate(this->trackUpdate);
    }
  }
}


// FlaggedAsyncUpdater

void EditContentComponent::handleAsyncUpdate()
{
  if (util::FlaggedAsyncUpdater::compareAndReset(this->trackUpdate))
  {
    this->updateTracks();
  }
}


// ApplicationCommandTarget

juce::ApplicationCommandTarget* EditContentComponent::getNextCommandTarget()
{
  return nullptr;
}

void EditContentComponent::getAllCommands(
    juce::Array<juce::CommandID>& commands)
{
  fillCommands(
      commands,
      CommandId::selectAll);
}

void EditContentComponent::getCommandInfo(
    juce::CommandID               commandID,
    juce::ApplicationCommandInfo& result)
{
  fillCommandInfo(
      result,
      commandID);
}

bool EditContentComponent::perform(
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
    ext::visit<VisitDepth::shallow>(
        *this->edit,
        [selectionManager](JuceTrack& track) {
          selectionManager->select(track, true);
        });
  }

  return true;
}

BLOOPER_NAMESPACE_END
