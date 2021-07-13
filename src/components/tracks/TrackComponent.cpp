#include <blooper/components/tracks/TrackComponent.hpp>

#include <blooper/internal/abstract/id.hpp>
#include <blooper/internal/ext/component.hpp>
#include <blooper/internal/ext/track.hpp>
#include <blooper/internal/utils/gui.hpp>

#include <blooper/context/behaviour/AssetManager.hpp>
#include <blooper/context/behaviour/EditManager.hpp>

#include <blooper/components/tracks/TrackPlayheadComponent.hpp>
#include <blooper/components/tracks/TrackButtonComponent.hpp>
#include <blooper/components/tracks/TrackPluginsComponent.hpp>
#include <blooper/components/tracks/TrackClipsComponent.hpp>

BLOOPER_NAMESPACE_BEGIN

TrackComponent::TrackComponent(
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
  auto& assets = this->getContext().getAssetManager();
  auto  undo = this->getContext().getUndoManagerPtr();


  // Appearance

  this->appearanceSettings =
      this->getContext()
          .getSettings()
          .getOrCreateChildWithName(
              id::appearance,
              nullptr);

  this->clipsSize.referTo(
      this->appearanceSettings,
      id::trackClipsSize,
      undo,
      100);


  // Header

  const auto trackName = this->track->getName();

  this->name =
      std::make_unique<juce::Label>(
          trackName + " Track name",
          trackName);

  TrackPlayheadOptions playheadOptions{};

  this->playhead =
      std::make_unique<TrackPlayheadComponent>(
          this->getContext(),
          this->getState().getOrCreateChildWithName(
              TrackPlayheadComponent::stateId,
              nullptr),
          this->track,
          move(playheadOptions));

  TrackButtonOptions buttonOptions{};

  this->button =
      std::make_unique<TrackButtonComponent>(
          this->getContext(),
          this->getState().getOrCreateChildWithName(
              TrackButtonComponent::stateId,
              nullptr),
          this->track,
          move(buttonOptions));


  // Small Buttons

  this->muteButton =
      std::make_unique<juce::DrawableButton>(
          "Mute",
          juce::DrawableButton::ImageFitted);

  this->muteButton->setImages(
      assets.getIconView(assets::IconAssetId::muteTrack));

  this->muteButton->setClickingTogglesState(true);

  this->muteButton->getToggleStateValue().referTo(
      this->track->state.getPropertyAsValue(
          te::IDs::mute,
          std::addressof(this->track->edit.getUndoManager())));

  this->soloButton =
      std::make_unique<juce::DrawableButton>(
          "Solo",
          juce::DrawableButton::ImageFitted);

  this->soloButton->setImages(
      assets.getIconView(assets::IconAssetId::soloTrack));

  this->soloButton->setClickingTogglesState(true);

  this->soloButton->getToggleStateValue().referTo(
      this->track->state.getPropertyAsValue(
          te::IDs::solo,
          std::addressof(this->track->edit.getUndoManager())));

  if (this->track->isAudioTrack())
  {
    this->armButton =
        std::make_unique<juce::DrawableButton>(
            "Arm",
            juce::DrawableButton::ImageFitted);

    this->armButton->setImages(
        assets.getIconView(assets::IconAssetId::armTrack));

    this->armButton->onClick = [this] {
      ext::toggleArmed(*this->track);

      this->armButton->setToggleState(
          ext::isArmed(*this->track),
          juce::dontSendNotification);
    };

    this->armButton->setToggleState(
        ext::isArmed(*this->track),
        juce::dontSendNotification);
  }


  // Dropdowns

  this->modeDropdown = std::make_unique<juce::ComboBox>();

  this->modeDropdown->addItem(
      "Sync",
      static_cast<int>(TrackMode::sync));

  this->modeDropdown->addItem(
      "One Shot",
      static_cast<int>(TrackMode::oneShot));

  this->modeDropdown->addItem(
      "Free",
      static_cast<int>(TrackMode::free));

  this->modeDropdown->setSelectedId(
      static_cast<int>(ext::getTrackMode(*this->track)),
      juce::dontSendNotification);


  this->intervalDropdown = std::make_unique<juce::ComboBox>();

  this->intervalDropdown->addItem(
      "1/4",
      static_cast<int>(Interval::quarterOfBeat));

  this->intervalDropdown->addItem(
      "1/2",
      static_cast<int>(Interval::halfOfBeat));

  this->intervalDropdown->addItem(
      "1",
      static_cast<int>(Interval::beat));

  this->intervalDropdown->addItem(
      "2",
      static_cast<int>(Interval::twoBeats));

  this->intervalDropdown->addItem(
      "4",
      static_cast<int>(Interval::fourBeats));

  this->intervalDropdown->addItem(
      "8",
      static_cast<int>(Interval::eightBeats));

  this->intervalDropdown->addItem(
      "16",
      static_cast<int>(Interval::sixteenBeats));

  this->intervalDropdown->setSelectedId(
      static_cast<int>(ext::getTrackInterval(*this->track)),
      juce::dontSendNotification);


  // Plugins

  TrackPluginsComponent::Options pluginListOptions{};

  pluginList =
      std::make_unique<TrackPluginsComponent>(
          this->getContext(),
          this->getState().getOrCreateChildWithName(
              TrackPluginsComponent::stateId,
              nullptr),
          this->track,
          move(pluginListOptions));


  // Footer

  if (auto clipTrack = ext::isClipTrack(this->track))
  {
    TrackClipsOptions clipsOptions{};

    this->clips =
        std::make_unique<TrackClipsComponent>(
            this->getContext(),
            this->getState().getOrCreateChildWithName(
                TrackClipsComponent::stateId,
                nullptr),
            clipTrack,
            move(clipsOptions));
  }


  // Add

  ext::addAndMakeVisible(
      *this,
      *this->name,
      *this->playhead,
      *this->button,

      *this->muteButton,
      *this->soloButton,
      *this->armButton,

      *this->modeDropdown,
      *this->intervalDropdown,

      *this->pluginList,
      *this->clips);


  // RAII

  this->track->state.addListener(this);

  this->name->addListener(this);

  this->modeDropdown->addListener(this);
  this->intervalDropdown->addListener(this);

  this->getContext().getSelectionManager().addChangeListener(this);

  this->appearanceSettings.addListener(this);
}

TrackComponent::~TrackComponent()
{
  this->appearanceSettings.removeListener(this);

  this->getContext().getSelectionManager().removeChangeListener(this);

  this->intervalDropdown->removeListener(this);
  this->modeDropdown->removeListener(this);

  this->name->removeListener(this);

  this->track->state.removeListener(this);
}


// Component

void TrackComponent::paint(JuceGraphics& g)
{
  util::drawOutline(
      g,
      *this,
      this->getContext()
          .getSelectionManager()
          .isSelected(
              this->track));
}

void TrackComponent::resized()
{
  auto availableArea =
      util::pad(
          this->getLocalBounds(),
          outlinePaddingFactor);


  // Header

  this->name->setBounds(
      availableArea.removeFromTop(
          static_cast<int>(
              this->getLookAndFeel()
                  .getLabelFont(*this->name)
                  .getHeight()) +
          4));

  this->button->setBounds(
      util::pad(availableArea.removeFromTop(
          availableArea.getWidth())));

  this->playhead->setBounds(
      util::pad(
          availableArea.removeFromTop(20)));

  availableArea.removeFromTop(10);


  // Small buttons

  auto bottomSmallButtonArea =
      availableArea
          .removeFromTop(availableArea.getWidth());

  auto topSmallButtonArea =
      bottomSmallButtonArea
          .removeFromTop(
              static_cast<int>(
                  bottomSmallButtonArea.getHeight() / 2));

  this->soloButton->setBounds(
      util::pad(
          topSmallButtonArea
              .removeFromLeft(
                  static_cast<int>(
                      topSmallButtonArea.getWidth() / 2))));

  this->muteButton->setBounds(
      util::pad(topSmallButtonArea));

  this->armButton->setBounds(
      util::pad(
          bottomSmallButtonArea
              .removeFromLeft(
                  static_cast<int>(
                      bottomSmallButtonArea.getWidth() / 2))));

  availableArea.removeFromTop(10);


  // Dropdowns

  this->modeDropdown->setBounds(
      util::pad(
          availableArea.removeFromTop(
              30),
          2));

  this->intervalDropdown->setBounds(
      util::pad(
          availableArea.removeFromTop(
              30),
          2));

  availableArea.removeFromTop(10);


  // Clips

  if (ext::isClipTrack(this->track))
  {
    this->clips->setBounds(
        availableArea.removeFromBottom(
            static_cast<int>(this->clipsSize)));

    availableArea.removeFromBottom(10);
  }


  // Plugin List

  this->pluginList->setBounds(availableArea);
}

void TrackComponent::mouseDown(const juce::MouseEvent& event)
{
  auto&      selection = this->getContext().getSelectionManager();
  const auto shouldAddToSelection = event.mods.isCtrlDown();
  const auto isSelected = selection.isSelected(this->track);

  if (shouldAddToSelection)
  {
    if (isSelected)
    {
      selection.deselect(this->track);
    }
    else
    {
      selection.select(this->track, true);
    }
  }
  else
  {
    selection.selectOnly(this->track);
  }
}


// ValueTreeListener

void TrackComponent::valueTreePropertyChanged(
    juce::ValueTree&        tree,
    const juce::Identifier& _id)
{
  if (tree.hasType(te::IDs::TRACK))
  {
    if (_id == te::IDs::NAME)
    {
      // TODO: this might update the text twice - fix
      this->name->setText(
          tree.getProperty(_id),
          juce::dontSendNotification);
    }
  }
  else if (tree == this->appearanceSettings)
  {
    if (_id == id::trackClipsSize)
    {
      this->resized();
    }
  }
}


// LabelListener

void TrackComponent::labelTextChanged(
    juce::Label* labelThatHasChanged)
{
  if (this->name.get() == labelThatHasChanged)
  {
    this->track->setName(this->name->getText());
  }
}


// ComboBoxListener

void TrackComponent::comboBoxChanged(
    juce::ComboBox* comboBoxThatHasChanged)
{
  if (comboBoxThatHasChanged == this->modeDropdown.get())
  {
    ext::setTrackMode(
        *this->track,
        static_cast<TrackMode>(
            this->modeDropdown->getSelectedId()));
  }
  else if (comboBoxThatHasChanged == this->intervalDropdown.get())
  {
    ext::setTrackInterval(
        *this->track,
        static_cast<Interval>(
            this->intervalDropdown->getSelectedId()));
  }
}


// ChangeListener

void TrackComponent::changeListenerCallback(
    juce::ChangeBroadcaster* broadcaster)
{
  if (broadcaster ==
      std::addressof(this->getContext().getSelectionManager()))
  {
    this->repaint();
  }
}

BLOOPER_NAMESPACE_END
