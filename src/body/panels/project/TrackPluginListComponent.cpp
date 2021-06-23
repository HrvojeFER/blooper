#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

TrackPluginListComponent::TrackPluginListComponent(
    AbstractContext& context,
    State            state,
    EditTrackRef     track,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options)),

      updatePluginsFlag(false),

      track(move(track))
{
  juce::ListBoxModel* model = this;

  this->list =
      std::make_unique<juce::ListBox>(
          this->track->getAudio().getName() + " Plugin List",
          model);

  ext::addAndMakeVisible(
      *this,
      *this->list);


  this->track->getAudio().state.addListener(this);
  this->getContext().registerCommandTarget(this);
}

TrackPluginListComponent::~TrackPluginListComponent()
{
  this->getContext().unregisterCommandTarget(this);
  this->track->getAudio().state.removeListener(this);
}


[[maybe_unused]] bool TrackPluginListComponent::isValidRow(int row)
{
  return row >= 0 && row < this->track->getAudio().pluginList.size();
}


// Component

void TrackPluginListComponent::resized()
{
  auto availableArea = this->getLocalBounds();

  this->list->setBounds(availableArea);
}


// ValueTreeListener

void TrackPluginListComponent::valueTreeChildAdded(
    juce::ValueTree& tree,
    juce::ValueTree& child)
{
  if (tree.hasType(te::IDs::TRACK))
    if (child.hasType(te::IDs::PLUGIN))
      this->markAndUpdate(this->updatePluginsFlag);
}

void TrackPluginListComponent::valueTreeChildRemoved(
    juce::ValueTree& tree,
    juce::ValueTree& child,
    int)
{
  if (tree.hasType(te::IDs::TRACK))
    if (child.hasType(te::IDs::PLUGIN))
      this->markAndUpdate(this->updatePluginsFlag);
}

void TrackPluginListComponent::valueTreeChildOrderChanged(
    juce::ValueTree& tree,
    int,
    int)
{
  if (tree.hasType(te::IDs::TRACK))
    this->markAndUpdate(this->updatePluginsFlag);
}


// ListBoxModel

int TrackPluginListComponent::getNumRows()
{
  return this->track->getAudio().pluginList.size();
}

void TrackPluginListComponent::paintListBoxItem(
    int             row,
    juce::Graphics& g,
    int             width,
    int             height,
    bool            isSelected)
{
  if (!this->isValidRow(row)) return;

  auto plugin = this->track->getAudio().pluginList[row];
  if (!plugin) return;

  auto availableArea = JuceBounds{0, 0, width, height};

  if (isSelected)
  {
    g.setColour(
        this->getLookAndFeel().findColour(
            ColourId::selection));
  }

  g.drawText(
      plugin->getName(),
      availableArea,
      juce::Justification::centredLeft,
      true);
}

void TrackPluginListComponent::listBoxItemClicked(
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

void TrackPluginListComponent::listBoxItemDoubleClicked(
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

juce::String TrackPluginListComponent::getTooltipForRow(
    int row)
{
  if (!this->isValidRow(row)) return {};
  auto plugin = this->track->getAudio().pluginList[row];

  return plugin->getSelectableDescription();
}


// FlaggedAsyncUpdater

void TrackPluginListComponent::handleAsyncUpdate()
{
  if (FlaggedAsyncUpdater::compareAndReset(this->updatePluginsFlag))
    this->list->updateContent();
}


// ApplicationCommandTarget

juce::ApplicationCommandTarget* TrackPluginListComponent::getNextCommandTarget()
{
  return nullptr;
}

void TrackPluginListComponent::getAllCommands(
    juce::Array<juce::CommandID>& commands)
{
  fillCommands(
      commands,
      CommandId::selectAll);
}

void TrackPluginListComponent::getCommandInfo(
    juce::CommandID               commandID,
    juce::ApplicationCommandInfo& result)
{
  fillCommandInfo(
      result,
      commandID);
}

bool TrackPluginListComponent::perform(
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
