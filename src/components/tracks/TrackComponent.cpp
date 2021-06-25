#include <blooper/components/tracks/TrackComponent.hpp>

#include <blooper/internal/ext/component.hpp>
#include <blooper/internal/utils/EditTrack.hpp>
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
    EditTrackRef     track,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options)),

      track(move(track))
{
  auto& assets = this->getContext().getAssetManager();


  // Header

  const auto trackName = this->track->getAudio().getName();

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

  this->muteButton->onClick = [this] {
    toggle(this->track->muted);
  };

  this->soloButton =
      std::make_unique<juce::DrawableButton>(
          "Solo",
          juce::DrawableButton::ImageFitted);

  this->soloButton->setImages(
      assets.getIconView(assets::IconAssetId::soloTrack));

  this->soloButton->onClick = [this] {
    this->track->toggleSoloed();
  };

  this->armButton =
      std::make_unique<juce::DrawableButton>(
          "Arm",
          juce::DrawableButton::ImageFitted);

  this->armButton->setImages(
      assets.getIconView(assets::IconAssetId::armTrack));

  this->armButton->onClick = [this] {
    toggle(this->track->armed);
  };


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
      static_cast<int>(this->track->mode.get()),
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
      static_cast<int>(this->track->interval.get()),
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

  TrackClipsOptions clipsOptions{};

  this->clips =
      std::make_unique<TrackClipsComponent>(
          this->getContext(),
          this->getState().getOrCreateChildWithName(
              TrackClipsComponent::stateId,
              nullptr),
          this->track,
          move(clipsOptions));


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

  this->track->getAudio().state.addListener(this);

  this->name->addListener(this);

  this->modeDropdown->addListener(this);
  this->intervalDropdown->addListener(this);
}

TrackComponent::~TrackComponent()
{
  this->intervalDropdown->removeListener(this);
  this->modeDropdown->removeListener(this);

  this->name->removeListener(this);

  this->track->getAudio().state.removeListener(this);
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

  const auto buttonArea =
      availableArea.removeFromTop(
                       availableArea.getWidth())
          .reduced(2);

  this->button->setBounds(buttonArea);
  this->playhead->setBounds(buttonArea);


  // Small buttons

  auto bottomSmallButtonArea =
      availableArea
          .removeFromTop(availableArea.getWidth())
          .reduced(2);

  auto topSmallButtonArea =
      bottomSmallButtonArea
          .removeFromTop(
              static_cast<int>(
                  bottomSmallButtonArea.getHeight() / 2));

  this->soloButton->setBounds(
      topSmallButtonArea
          .removeFromLeft(
              static_cast<int>(
                  topSmallButtonArea.getWidth() / 2))
          .reduced(4));

  this->muteButton->setBounds(
      topSmallButtonArea
          .reduced(4));

  this->armButton->setBounds(
      bottomSmallButtonArea
          .removeFromLeft(
              static_cast<int>(
                  bottomSmallButtonArea.getWidth() / 2))
          .reduced(4));


  // Dropdowns

  this->modeDropdown->setBounds(
      availableArea.removeFromTop(
          30));

  this->intervalDropdown->setBounds(
      availableArea.removeFromTop(
          30));


  // Plugin List

  this->pluginList->setBounds(availableArea);


  // TODO: Footer
}

void TrackComponent::mouseDown(const juce::MouseEvent&)
{
  this->getContext()
      .getSelectionManager()
      .select(
          this->track,
          true);
}


// ValueTreeListener

void TrackComponent::valueTreePropertyChanged(
    juce::ValueTree&        tree,
    const juce::Identifier& id)
{
  if (tree.hasType(te::IDs::TRACK))
  {
    if (id == te::IDs::NAME)
    {
      // TODO: this might update the text twice - fix
      this->name->setText(
          tree.getProperty(id),
          juce::dontSendNotification);
    }
  }
}


// LabelListener

void TrackComponent::labelTextChanged(
    juce::Label* labelThatHasChanged)
{
  if (this->name.get() == labelThatHasChanged)
  {
    this->track->getAudio().setName(this->name->getText());
  }
}

// ComboBoxListener

void TrackComponent::comboBoxChanged(
    juce::ComboBox* comboBoxThatHasChanged)
{
  if (comboBoxThatHasChanged == this->modeDropdown.get())
  {
    this->track->mode = static_cast<TrackMode>(
        this->modeDropdown->getSelectedId());
  }
  else if (comboBoxThatHasChanged == this->intervalDropdown.get())
  {
    this->track->interval = static_cast<Interval>(
        this->intervalDropdown->getSelectedId());
  }
}

BLOOPER_NAMESPACE_END
