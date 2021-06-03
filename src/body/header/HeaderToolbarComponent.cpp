#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

class HeaderToolbarComponent::ItemFactory :
    public juce::ToolbarItemFactory
{
 public:
  explicit ItemFactory(HeaderToolbarComponent* parent) : parent(parent) { }


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

    addTrackButtonId = 9,
    addPluginButtonId = 10,

    showMasterButtonId = 11,
    showControlSurfaceButtonId = 12,
    showBrowserButtonId = 13,
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

    ids.add(addTrackButtonId);
    ids.add(addPluginButtonId);

    ids.add(showMasterButtonId);
    ids.add(showControlSurfaceButtonId);
    ids.add(showBrowserButtonId);
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

    ids.add(addTrackButtonId);
    ids.add(addPluginButtonId);

    ids.add(spacerId);

    ids.add(showMasterButtonId);
    ids.add(showControlSurfaceButtonId);
    ids.add(showBrowserButtonId);
  }


  juce::ToolbarItemComponent* createItem(int itemId) override
  {
    auto& assetManager = this->parent->getContext().getAssetManager();

    juce::ToolbarButton* result;
    switch (itemId)
    {
      case playButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Play",
            assetManager.getDrawable(
                assets::DrawableAssetId::playIcon),
            {});
        break;

      case pauseButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Pause",
            assetManager.getDrawable(
                assets::DrawableAssetId::pauseIcon),
            {});
        break;

      case stopButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Stop",
            assetManager.getDrawable(
                assets::DrawableAssetId::stopIcon),
            {});
        break;

      case recordButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Record",
            assetManager.getDrawable(
                assets::DrawableAssetId::recordIcon),
            {});
        break;


      case muteTrackButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Mute",
            assetManager.getDrawable(
                assets::DrawableAssetId::notImplementedIcon),
            {});
        break;

      case soloTrackButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Solo",
            assetManager.getDrawable(
                assets::DrawableAssetId::notImplementedIcon),
            {});
        break;

      case armTrackButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Arm",
            assetManager.getDrawable(
                assets::DrawableAssetId::notImplementedIcon),
            {});
        break;


      case addTrackButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Add Track",
            assetManager.getDrawable(
                assets::DrawableAssetId::notImplementedIcon),
            {});
        break;

      case addPluginButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Add Plugin",
            assetManager.getDrawable(
                assets::DrawableAssetId::notImplementedIcon),
            {});
        break;


      case showMasterButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Show Master Track",
            assetManager.getDrawable(
                assets::DrawableAssetId::notImplementedIcon),
            {});
        break;

      case showControlSurfaceButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Show Control Surface",
            assetManager.getDrawable(
                assets::DrawableAssetId::notImplementedIcon),
            {});
        break;

      case showBrowserButtonId:
        result = new juce::ToolbarButton(
            itemId,
            "Show Browser",
            assetManager.getDrawable(
                assets::DrawableAssetId::notImplementedIcon),
            {});
        break;


      default:
        result = nullptr;
    }

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
