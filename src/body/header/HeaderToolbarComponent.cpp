#include <blooper/body/header/HeaderToolbarComponent.hpp>

#include <blooper/internal/ext/component.hpp>
#include <blooper/internal/utils/ContextCommands.hpp>

#include <blooper/context/behaviour/AssetManager.hpp>

BLOOPER_NAMESPACE_BEGIN

// ItemFactory

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

    // Edit

    deleteButtonId = CommandId::del,
    cutButtonId = CommandId::cut,
    copyButtonId = CommandId::copy,
    pasteButtonId = CommandId::paste,

    undoButtonId = CommandId::undo,
    redoButtonId = CommandId::redo,

    addEditButtonId = CommandId::addEdit,
    addTrackButtonId = CommandId::addTrack,
    addPluginButtonId = CommandId::addPlugin,

    // Transport

    togglePlayingButtonId = CommandId::togglePlaying,
    toggleRecordingButtonId = CommandId::toggleRecording,

    cycleTrackModeButtonId = CommandId::cycleTrackMode,
    cycleTrackIntervalButtonId = CommandId::cycleTrackInterval,

    // Tracks

    toggleMutedButtonId = CommandId::toggleMuted,
    toggleSoloedButtonId = CommandId::toggleSoloed,
    toggleArmedButtonId = CommandId::toggleArmed,

    // Parameters

    nudgeUpButtonId = CommandId::nudgeUp,
    nudgeDownButtonId = CommandId::nudgeDown,
  };


  void getAllToolbarItemIds(juce::Array<int>& ids) override
  {
    // JUCE IDs

    ids.add(separatorBarId);
    ids.add(spacerId);
    ids.add(flexibleSpacerId);


    // Blooper IDs

    // Edit

    ids.add(deleteButtonId);
    ids.add(cutButtonId);
    ids.add(copyButtonId);
    ids.add(pasteButtonId);

    ids.add(undoButtonId);
    ids.add(redoButtonId);

    ids.add(addEditButtonId);
    ids.add(addTrackButtonId);
    ids.add(addPluginButtonId);

    // Transport

    ids.add(togglePlayingButtonId);
    ids.add(toggleRecordingButtonId);

    // Tracks

    ids.add(toggleMutedButtonId);
    ids.add(toggleSoloedButtonId);
    ids.add(toggleArmedButtonId);

    ids.add(cycleTrackModeButtonId);
    ids.add(cycleTrackIntervalButtonId);

    // Parameters

    ids.add(nudgeUpButtonId);
    ids.add(nudgeDownButtonId);
  }


  void getDefaultItemSet(juce::Array<int>& ids) override
  {
    ids.add(togglePlayingButtonId);
    ids.add(toggleRecordingButtonId);

    ids.add(spacerId);

    ids.add(toggleMutedButtonId);
    ids.add(toggleSoloedButtonId);
    ids.add(toggleArmedButtonId);

    ids.add(spacerId);

    ids.add(cycleTrackModeButtonId);
    ids.add(cycleTrackIntervalButtonId);

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

    ids.add(addEditButtonId);
    ids.add(addTrackButtonId);
    ids.add(addPluginButtonId);
  }


  juce::ToolbarItemComponent* createItem(int itemId) override
  {
    auto& assetManager = this->parent->getContext().getAssetManager();

#ifdef __JETBRAINS_IDE__
  #pragma clang diagnostic push
  #pragma ide diagnostic   ignored "UnusedValue"
#endif //__JETBRAINS_IDE__
    juce::ToolbarButton* result{nullptr};
    JuceCommandId        commandId{CommandId::none};
#ifdef __JETBRAINS_IDE__
  #pragma clang diagnostic pop
#endif //__JETBRAINS_IDE__

    switch (itemId)
    {
      case togglePlayingButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Toggle Playing",
            assetManager.getIcon(
                assets::IconAssetId::play),
            {});
        commandId = CommandId::togglePlaying;
        break;

      case toggleRecordingButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Toggle Recording",
            assetManager.getIcon(
                assets::IconAssetId::record),
            {});
        commandId = CommandId::toggleRecording;
        break;


      case toggleMutedButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Toggle Muted",
            assetManager.getIcon(
                assets::IconAssetId::muteTrack),
            {});
        commandId = CommandId::toggleMuted;
        break;

      case toggleSoloedButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Toggle Soloed",
            assetManager.getIcon(
                assets::IconAssetId::soloTrack),
            {});
        commandId = CommandId::toggleSoloed;
        break;

      case toggleArmedButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Toggle Armed",
            assetManager.getIcon(
                assets::IconAssetId::armTrack),
            {});
        commandId = CommandId::toggleArmed;
        break;


      case cycleTrackModeButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Cycle Track Mode",
            assetManager.getIcon(
                assets::IconAssetId::cycleTrackMode),
            {});
        commandId = CommandId::cycleTrackMode;
        break;

      case cycleTrackIntervalButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Cycle Track Interval",
            assetManager.getIcon(
                assets::IconAssetId::cycleTrackInterval),
            {});
        commandId = CommandId::cycleTrackInterval;
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


      case addEditButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Add Edit",
            assetManager.getIcon(
                assets::IconAssetId::addEdit),
            {});
        commandId = CommandId::addEdit;
        break;

      case addTrackButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Add Track",
            assetManager.getIcon(
                assets::IconAssetId::addTrack),
            {});
        commandId = CommandId::addTrack;
        break;

      case addPluginButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Add Plugin",
            assetManager.getIcon(
                assets::IconAssetId::addPlugin),
            {});
        commandId = CommandId::addPlugin;
        break;


      default:
        break;
    }

#ifdef __JETBRAINS_IDE__
  #pragma clang diagnostic push
  #pragma ide diagnostic   ignored "ConstantConditionsOC"
#endif //__JETBRAINS_IDE__
    if (result && commandId != CommandId::none)
#ifdef __JETBRAINS_IDE__
  #pragma clang diagnostic pop
#endif //__JETBRAINS_IDE__
    {
      result->setCommandToTrigger(
          std::addressof(
              this->parent->getContext().getCommandManager()),
          static_cast<JuceCommandId>(commandId),
          true);
    }

    return result;
  }

 private:
  [[maybe_unused]] HeaderToolbarComponent* parent;
};


// Toolbar

HeaderToolbarComponent::HeaderToolbarComponent(
    AbstractContext& context,
    State            state,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
  auto& assets = this->getContext().getAssetManager();


  this->factory =
      std::make_unique<HeaderToolbarComponent::ItemFactory>(
          this);

  this->toolbar =
      std::make_unique<juce::Toolbar>();

  this->toolbarCustomizationButton =
      std::make_unique<juce::DrawableButton>(
          JuceString{"Customize Toolbar"},
          juce::DrawableButton::ButtonStyle::ImageFitted);

  this->toolbarCustomizationButton->setImages(
      assets.getIconView(assets::IconAssetId::menu));

  this->toolbarCustomizationButton->onClick = [this] {
    this->toolbar->showCustomisationDialog(*this->factory);
  };


  auto toolbarState =
      this->getContext()
          .getSettings()
          .getProperty(HeaderToolbarComponent::toolbarStateId)
          .toString();

  if (toolbarState.isEmpty())
  {
    this->toolbar->addDefaultItems(*this->factory);
  }
  else
  {
    this->toolbar->restoreFromString(*this->factory, toolbarState);
  }


  ext::addAndMakeVisible(
      *this,
      *this->toolbar,
      *this->toolbarCustomizationButton);
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


// Component

void HeaderToolbarComponent::resized()
{
  auto availableArea = this->getLocalBounds();


  this->toolbarCustomizationButton->setBounds(
      availableArea.removeFromRight(
          availableArea.getHeight()));

  this->toolbar->setBounds(availableArea);
}

BLOOPER_NAMESPACE_END
