#include <blooper/components/tracks/TrackClipsComponent.hpp>

#include <blooper/internal/ext/component.hpp>
#include <blooper/internal/utils/ContextCommands.hpp>
#include <blooper/internal/utils/style.hpp>
#include <blooper/internal/utils/gui.hpp>

BLOOPER_NAMESPACE_BEGIN

TrackClipsComponent::TrackClipsComponent(
    AbstractContext&  context,
    State             state,
    JuceClipTrackRef  track,
    TrackClipsOptions options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options)),

      track(move(track))
{
  this->list =
      std::make_unique<juce::ListBox>(
          this->track->getName() + " Clips",
          static_cast<juce::ListBoxModel*>(this));

  this->list->setRowHeight(35);


  ext::addAndMakeVisible(
      *this,
      *this->list);


  this->track->state.addListener(this);
  this->getContext().registerCommandTarget(this);
}

TrackClipsComponent::~TrackClipsComponent()
{
  this->getContext().unregisterCommandTarget(this);
  this->track->state.removeListener(this);
}


bool TrackClipsComponent::isValidRow(int row) const noexcept
{
  return row >= 0 && row < this->track->getClips().size();
}


// Component

void TrackClipsComponent::resized()
{
  auto availableArea = this->getLocalBounds();

  this->list->setBounds(availableArea);
}


// ListBoxModel

int TrackClipsComponent::getNumRows()
{
  return this->track->getClips().size();
}

void TrackClipsComponent::paintListBoxItem(
    int             row,
    juce::Graphics& g,
    int             width,
    int             height,
    bool            isSelected)
{
  if (!this->isValidRow(row)) return;

  const auto clip = this->track->getClips()[row];
  if (!clip) return;

  isSelected = false;
  if (auto selection =
          this->getContext()
              .getEngine()
              .getUIBehaviour()
              .getCurrentlyFocusedSelectionManager())
  {
    isSelected = selection->isSelected(clip);
  }

  auto availableArea =
      util::drawBottomLine(
          g,
          *this,
          {0,
           0,
           width,
           height},
          isSelected);

  if (isSelected)
  {
    g.setColour(
        this->findColour(
            ColourId::selection));
  }
  else
  {
    g.setColour(
        this->findColour(
            ColourId::white));
  }

  g.drawText(
      clip->getName(),
      availableArea,
      juce::Justification::centredLeft);
}

void TrackClipsComponent::listBoxItemClicked(
    int                     row,
    const juce::MouseEvent& event)
{
  if (!this->isValidRow(row)) return;

  const auto clip = this->track->getClips()[row];
  if (!clip) return;

  if (auto selection =
          this->getContext()
              .getEngine()
              .getUIBehaviour()
              .getCurrentlyFocusedSelectionManager())
  {
    selection->select(clip, event.mods.isCtrlDown());
  }
}

void TrackClipsComponent::listBoxItemDoubleClicked(
    int row,
    const juce::MouseEvent&)
{
  if (!this->isValidRow(row)) return;

  const auto clip = this->track->getClips()[row];
  if (!clip) return;

  if (auto selection =
          this->getContext()
              .getEngine()
              .getUIBehaviour()
              .getCurrentlyFocusedSelectionManager())
  {
    selection->selectOnly(clip);
  }
}

juce::String TrackClipsComponent::getTooltipForRow(
    int row)
{
  if (!this->isValidRow(row)) return {};

  const auto clip = this->track->getClips()[row];
  if (!clip) return {};

  return clip->getSelectableDescription() +
         "\n" +
         "Takes: " + JuceString(clip->getNumTakes(false)) +
         "\n" +
         "Start: " + JuceString(clip->getStartBeat()) +
         "\n" +
         "End: " + JuceString(clip->getEndBeat());
}


// ApplicationCommandTarget

juce::ApplicationCommandTarget* TrackClipsComponent::getNextCommandTarget()
{
  return nullptr;
}

void TrackClipsComponent::getAllCommands(
    juce::Array<juce::CommandID>& commands)
{
  fillCommands(
      commands,
      CommandId::selectAll);
}

void TrackClipsComponent::getCommandInfo(
    juce::CommandID               commandID,
    juce::ApplicationCommandInfo& result)
{
  fillCommandInfo(
      result,
      commandID);
}

bool TrackClipsComponent::perform(
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
    for (auto clip : this->track->getClips())
      selectionManager->select(clip, true);
  }

  return true;
}


// ValueTreeListener

void TrackClipsComponent::valueTreeChildAdded(
    juce::ValueTree& tree,
    juce::ValueTree& child)
{
  if (tree == this->track->state)
  {
    if (child.hasType(te::IDs::AUDIOCLIP))
    {
      this->list->updateContent();
    }
  }
}

void TrackClipsComponent::valueTreeChildRemoved(
    juce::ValueTree& tree,
    juce::ValueTree& child,
    int)
{
  if (tree == this->track->state)
  {
    if (child.hasType(te::IDs::AUDIOCLIP))
    {
      this->list->updateContent();
    }
  }
}

void TrackClipsComponent::valueTreeChildOrderChanged(
    juce::ValueTree& tree,
    int              childAIndex,
    int              childBIndex)
{
  if (tree == this->track->state)
  {
    if (tree.getChild(childAIndex).hasType(te::IDs::AUDIOCLIP) ||
        tree.getChild(childBIndex).hasType(te::IDs::AUDIOCLIP))
    {
      this->list->updateContent();
    }
  }
}

BLOOPER_NAMESPACE_END
