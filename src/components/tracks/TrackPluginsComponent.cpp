#include <blooper/components/tracks/TrackPluginsComponent.hpp>

#include <blooper/internal/ext/component.hpp>
#include <blooper/internal/utils/ContextCommands.hpp>
#include <blooper/internal/utils/style.hpp>
#include <blooper/internal/utils/gui.hpp>

#include <blooper/context/behaviour/AssetManager.hpp>

BLOOPER_NAMESPACE_BEGIN

TrackPluginsComponent::TrackPluginsComponent(
    AbstractContext& context,
    State            state,
    JuceTrackRef     track,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options)),

      track(move(track))
{
  this->list =
      std::make_unique<juce::ListBox>(
          this->track->getName() + " Plugin List",
          static_cast<juce::ListBoxModel*>(this));

  this->list->setRowHeight(35);


  ext::addAndMakeVisible(
      *this,
      *this->list);


  this->track->state.addListener(this);
  this->getContext().registerCommandTarget(this);
}

TrackPluginsComponent::~TrackPluginsComponent()
{
  this->getContext().unregisterCommandTarget(this);
  this->track->state.removeListener(this);
}


[[maybe_unused]] bool TrackPluginsComponent::isValidRow(int row)
{
  return row >= 0 && row < this->track->pluginList.size();
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
  if (tree == this->track->state)
  {
    if (child.hasType(te::IDs::PLUGIN))
    {
      this->list->updateContent();
    }
  }
}

void TrackPluginsComponent::valueTreeChildRemoved(
    juce::ValueTree& tree,
    juce::ValueTree& child,
    int)
{
  if (tree == this->track->state)
  {
    if (child.hasType(te::IDs::PLUGIN))
    {
      this->list->updateContent();
    }
  }
}

void TrackPluginsComponent::valueTreeChildOrderChanged(
    juce::ValueTree& tree,
    int              childAIndex,
    int              childBIndex)
{
  if (tree == this->track->state)
  {
    if (tree.getChild(childAIndex).hasType(te::IDs::PLUGIN))
    {
      this->list->updateContent();
    }
    if (tree.getChild(childBIndex).hasType(te::IDs::PLUGIN))
    {
      this->list->updateContent();
    }
  }
}


// ListBoxModel

int TrackPluginsComponent::getNumRows()
{
  return this->track->pluginList.size();
}

void TrackPluginsComponent::paintListBoxItem(
    int             row,
    juce::Graphics& g,
    int             width,
    int             height,
    bool            isSelected)
{
  if (!this->isValidRow(row)) return;

  auto plugin = this->track->pluginList[row];
  if (!plugin) return;

  isSelected = false;
  if (auto selection =
          this->getContext()
              .getEngine()
              .getUIBehaviour()
              .getCurrentlyFocusedSelectionManager())
  {
    isSelected = selection->isSelected(plugin);
  }

  auto availableArea =
      util::drawBottomLine(
          g,
          *this,
          JuceBounds{
              0,
              0,
              width,
              height},
          isSelected);

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
    int                     row,
    const juce::MouseEvent& event)
{
  if (!this->isValidRow(row)) return;

  auto plugin = this->track->pluginList[row];
  if (!plugin) return;

  if (auto selectionManager =
          this->getContext()
              .getEngine()
              .getUIBehaviour()
              .getCurrentlyFocusedSelectionManager())
  {
    selectionManager->select(plugin, event.mods.isCtrlDown());
  }
}

void TrackPluginsComponent::listBoxItemDoubleClicked(
    int row,
    const juce::MouseEvent&)
{
  if (!this->isValidRow(row)) return;

  auto plugin = this->track->pluginList[row];
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
  auto plugin = this->track->pluginList[row];

  return plugin->getSelectableDescription();
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
    for (auto plugin : this->track->pluginList)
      selectionManager->select(plugin, true);
  }

  return true;
}

BLOOPER_NAMESPACE_END
