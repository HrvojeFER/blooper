#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

class HeaderToolbarComponent::ItemFactory :
    public juce::ToolbarItemFactory
{
 public:
  explicit ItemFactory(HeaderToolbarComponent* parent)
      : parent(parent)
  {
  }


  enum Ids : JuceToolbarItemId
  {
    // Copied from JUCE

    // The item ID for a vertical (or horizontal) separator bar that can be
    // placed between sets of items to break them into groups.
    separatorBarId = -1,

    // The item ID for a fixed-width space that can be placed between items.
    spacerId = -2,

    // The item ID for a gap that pushes outwards against the things on
    // either side of it, filling any available space.
    flexibleSpacerId = -3,


    // blooper IDs

    playButtonId = 1,
    pauseButtonId = 2,
    stopButtonId = 3,
    recordButtonId = 4,

    muteTrackButtonId = 5,
    soloTrackButtonId = 6,
    armTrackButtonId = 7,

    nudgeUpButtonId = 9,
    nudgeDownButtonId = 10,

    deleteButtonId = 11,
    cutButtonId = 12,
    copyButtonId = 13,
    pasteButtonId = 14,

    undoButtonId = 15,
    redoButtonId = 16,

    addTrackButtonId = 17,
    addPluginButtonId = 18,
  };


  void getAllToolbarItemIds(juce::Array<int>& ids) override
  {
    // JUCE IDs

    ids.add(separatorBarId);
    ids.add(spacerId);
    ids.add(flexibleSpacerId);


    // Blooper IDs

    ids.add(playButtonId);
    ids.add(pauseButtonId);
    ids.add(stopButtonId);
    ids.add(recordButtonId);

    ids.add(muteTrackButtonId);
    ids.add(soloTrackButtonId);
    ids.add(armTrackButtonId);

    ids.add(nudgeUpButtonId);
    ids.add(nudgeDownButtonId);

    ids.add(deleteButtonId);
    ids.add(cutButtonId);
    ids.add(copyButtonId);
    ids.add(pasteButtonId);

    ids.add(undoButtonId);
    ids.add(redoButtonId);

    ids.add(addTrackButtonId);
    ids.add(addPluginButtonId);
  }


  void getDefaultItemSet(juce::Array<int>& ids) override
  {
    ids.add(playButtonId);
    ids.add(pauseButtonId);
    ids.add(stopButtonId);
    ids.add(recordButtonId);

    ids.add(spacerId);

    ids.add(muteTrackButtonId);
    ids.add(soloTrackButtonId);
    ids.add(armTrackButtonId);

    ids.add(spacerId);

    ids.add(nudgeUpButtonId);
    ids.add(nudgeDownButtonId);


    ids.add(separatorBarId);

    ids.add(deleteButtonId);
    ids.add(cutButtonId);
    ids.add(copyButtonId);
    ids.add(pasteButtonId);

    ids.add(spacerId);

    ids.add(undoButtonId);
    ids.add(redoButtonId);


    ids.add(separatorBarId);

    ids.add(addTrackButtonId);
    ids.add(addPluginButtonId);
  }


  juce::ToolbarItemComponent* createItem(int itemId) override
  {
    auto& assetManager = this->parent->getContext().getAssetManager();

    juce::ToolbarButton* result;
    JuceCommandId        commandId;

    switch (itemId)
    {
      case playButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Play",
            assetManager.getIcon(
                assets::IconAssetId::play),
            {});
        commandId = CommandId::play;
        break;

      case pauseButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Pause",
            assetManager.getIcon(
                assets::IconAssetId::pause),
            {});
        commandId = CommandId::pause;
        break;

      case stopButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Stop",
            assetManager.getIcon(
                assets::IconAssetId::stop),
            {});
        commandId = CommandId::stop;
        break;

      case recordButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Record",
            assetManager.getIcon(
                assets::IconAssetId::record),
            {});
        commandId = CommandId::record;
        break;


      case muteTrackButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Mute",
            assetManager.getIcon(
                assets::IconAssetId::muteTrack),
            {});
        commandId = CommandId::muteTrack;
        break;

      case soloTrackButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Solo",
            assetManager.getIcon(
                assets::IconAssetId::soloTrack),
            {});
        commandId = CommandId::soloTrack;
        break;

      case armTrackButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Arm",
            assetManager.getIcon(
                assets::IconAssetId::armTrack),
            {});
        commandId = CommandId::armTrack;
        break;


      case nudgeUpButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Nudge Up",
            assetManager.getIcon(
                assets::IconAssetId::nudgeUp),
            {});
        commandId = CommandId::nudgeUp;
        break;

      case nudgeDownButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Nudge Down",
            assetManager.getIcon(
                assets::IconAssetId::nudgeDown),
            {});
        commandId = CommandId::nudgeDown;
        break;


      case deleteButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Delete",
            assetManager.getIcon(
                assets::IconAssetId::del),
            {});
        commandId = CommandId::del;
        break;

      case cutButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Cut",
            assetManager.getIcon(
                assets::IconAssetId::cut),
            {});
        commandId = CommandId::cut;
        break;

      case copyButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Copy",
            assetManager.getIcon(
                assets::IconAssetId::copy),
            {});
        commandId = CommandId::copy;
        break;

      case pasteButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Paste",
            assetManager.getIcon(
                assets::IconAssetId::paste),
            {});
        commandId = CommandId::paste;
        break;


      case undoButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Undo",
            assetManager.getIcon(
                assets::IconAssetId::undo),
            {});
        commandId = CommandId::undo;
        break;

      case redoButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Redo",
            assetManager.getIcon(
                assets::IconAssetId::redo),
            {});
        commandId = CommandId::redo;
        break;


      case addTrackButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Add Track",
            assetManager.getIcon(
                assets::IconAssetId::add),
            {});
        commandId = CommandId::addTrack;
        break;

      case addPluginButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Add Plugin",
            assetManager.getIcon(
                assets::IconAssetId::addOther),
            {});
        commandId = CommandId::addPlugin;
        break;


      default:
        result = nullptr;
        commandId = CommandId::none;
    }

    if (result)
      result->setCommandToTrigger(
          std::addressof(
              this->parent->getContext().getCommandManager()),
          static_cast<JuceCommandId>(commandId),
          true);

    return result;
  }

 private:
  [[maybe_unused]] HeaderToolbarComponent* parent;
};

HeaderToolbarComponent::HeaderToolbarComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
  this->toolbar =
      std::make_unique<juce::Toolbar>();

  this->factory =
      std::make_unique<HeaderToolbarComponent::ItemFactory>(
          this);


  auto toolbarState =
      this->getContext()
          .getSettings()
          .getProperty(HeaderToolbarComponent::toolbarStateId)
          .toString();

  if (toolbarState.isEmpty())
  {
    this->toolbar->addDefaultItems(*factory);
  }
  else
  {
    this->toolbar->restoreFromString(*factory, toolbarState);
  }


  ext::addAndMakeVisible(
      *this,
      *this->toolbar);
}

HeaderToolbarComponent::~HeaderToolbarComponent()
{
  this->getContext()
      .getSettings()
      .setProperty(
          HeaderToolbarComponent::toolbarStateId,
          this->toolbar->toString(),
          nullptr);
}


void HeaderToolbarComponent::resized()
{
  auto availableArea = this->getLocalBounds();

  this->toolbar->setBounds(availableArea);
}

BLOOPER_NAMESPACE_END
