#include <blooper/components/tracks/TrackPluginsComponent.hpp>

#include <blooper/internal/ext/component.hpp>
#include <blooper/internal/utils/ContextCommands.hpp>
#include <blooper/internal/utils/EditTrack.hpp>
#include <blooper/internal/utils/gui.hpp>
#include <blooper/internal/utils/style.hpp>

#include <blooper/context/behaviour/AssetManager.hpp>

BLOOPER_NAMESPACE_BEGIN

TrackPluginsComponent::TrackPluginsComponent(
    AbstractContext& context,
    State            state,
    EditTrackRef     track,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options)),

      track(move(track)),

      pluginUpdate(false)
{
  this->list =
      std::make_unique<juce::ListBox>(
          this->track->getAudio().getName() + " Plugin List",
          dynamic_cast<juce::ListBoxModel*>(this));


  ext::addAndMakeVisible(
      *this,
      *this->list);


  this->track->getAudio().state.addListener(this);
  this->getContext().registerCommandTarget(this);
}

TrackPluginsComponent::~TrackPluginsComponent()
{
  this->getContext().unregisterCommandTarget(this);
  this->track->getAudio().state.removeListener(this);
}


[[maybe_unused]] bool TrackPluginsComponent::isValidRow(int row)
{
  return row >= 0 && row < this->track->getAudio().pluginList.size();
}


void TrackPluginsComponent::updatePlugins()
{
  this->list->updateContent();
}


// Component

void TrackPluginsComponent::resized()
{
  auto availableArea = this->getLocalBounds();

  this->list->setBounds(availableArea);
}


// ValueTreeListener

void TrackPluginsComponent::valueTreeChildAdded(
    juce::ValueTree& tree,
    juce::ValueTree& child)
{
  if (tree.hasType(te::IDs::TRACK))
  {
    if (child.hasType(te::IDs::PLUGIN))
    {
      this->markAndUpdate(this->pluginUpdate);
    }
  }
}

void TrackPluginsComponent::valueTreeChildRemoved(
    juce::ValueTree& tree,
    juce::ValueTree& child,
    int)
{
  if (tree.hasType(te::IDs::TRACK))
  {
    if (child.hasType(te::IDs::PLUGIN))
    {
      this->markAndUpdate(this->pluginUpdate);
    }
  }
}

void TrackPluginsComponent::valueTreeChildOrderChanged(
    juce::ValueTree& tree,
    int              childAIndex,
    int              childBIndex)
{
  if (tree.hasType(te::IDs::TRACK))
  {
    if (tree.getChild(childAIndex).hasType(te::IDs::PLUGIN))
    {
      this->markAndUpdate(this->pluginUpdate);
    }
    if (tree.getChild(childBIndex).hasType(te::IDs::PLUGIN))
    {
      this->markAndUpdate(this->pluginUpdate);
    }
  }
}


// ListBoxModel

int TrackPluginsComponent::getNumRows()
{
  return this->track->getAudio().pluginList.size();
}

void TrackPluginsComponent::paintListBoxItem(
    int             row,
    juce::Graphics& g,
    int             width,
    int             height,
    bool            isSelected)
{
  if (!this->isValidRow(row)) return;

  auto plugin = this->track->getAudio().pluginList[row];
  if (!plugin) return;

  auto availableArea =
      util::drawBottomLine(
          g,
          *this,
          JuceBounds{
              0,
              0,
              width,
              height},
          this->getContext()
              .getSelectionManager()
              .isSelected(plugin));

  if (isSelected)
  {
    g.setColour(
        this->getLookAndFeel().findColour(
            ColourId::selection));
  }
  else
  {
    g.setColour(
        this->getLookAndFeel().findColour(
            ColourId::white));
  }

  g.drawText(
      plugin->getName(),
      availableArea,
      juce::Justification::centredLeft,
      true);
}

void TrackPluginsComponent::listBoxItemClicked(
    int row,
    const juce::MouseEvent&)
{
  if (!this->isValidRow(row)) return;
  auto plugin = this->track->getAudio().pluginList[row];

  if (auto selectionManager =
          this->getContext()
              .getEngine()
              .getUIBehaviour()
              .getCurrentlyFocusedSelectionManager())
  {
    if (selectionManager->isSelected(plugin))
    {
      selectionManager->deselect(plugin);
    }
    else
    {
      selectionManager->select(plugin, true);
    }
  }
}

void TrackPluginsComponent::listBoxItemDoubleClicked(
    int row,
    const juce::MouseEvent&)
{
  if (!this->isValidRow(row)) return;

  auto plugin = this->track->getAudio().pluginList[row];
  if (!plugin) return;

  if (auto selectionManager =
          this->getContext()
              .getEngine()
              .getUIBehaviour()
              .getCurrentlyFocusedSelectionManager())
  {
    selectionManager->selectOnly(plugin);
  }

  plugin->showWindowExplicitly();
}

juce::String TrackPluginsComponent::getTooltipForRow(int row)
{
  if (!this->isValidRow(row)) return {};
  auto plugin = this->track->getAudio().pluginList[row];

  return plugin->getSelectableDescription();
}


// FlaggedAsyncUpdater

void TrackPluginsComponent::handleAsyncUpdate()
{
  if (FlaggedAsyncUpdater::compareAndReset(this->pluginUpdate))
  {
    this->updatePlugins();
  }
}


// ApplicationCommandTarget

juce::ApplicationCommandTarget* TrackPluginsComponent::getNextCommandTarget()
{
  return nullptr;
}

void TrackPluginsComponent::getAllCommands(
    juce::Array<juce::CommandID>& commands)
{
  fillCommands(
      commands,
      CommandId::selectAll);
}

void TrackPluginsComponent::getCommandInfo(
    juce::CommandID               commandID,
    juce::ApplicationCommandInfo& result)
{
  fillCommandInfo(
      result,
      commandID);
}

bool TrackPluginsComponent::perform(
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
    for (auto plugin : this->track->getAudio().pluginList)
      selectionManager->select(plugin, true);
  }

  this->list->selectRangeOfRows(
      0,
      this->track->getAudio().pluginList.size());

  return true;
}

BLOOPER_NAMESPACE_END
