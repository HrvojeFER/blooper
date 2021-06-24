#include <blooper/body/panels/project/EditTrackComponent.hpp>

#include <blooper/body/panels/project/TrackPluginListComponent.hpp>

#include <blooper/internal/abstract/id.hpp>
#include <blooper/internal/ext/component.hpp>
#include <blooper/internal/utils/style.hpp>

#include <blooper/context/behaviour/AssetManager.hpp>
#include <blooper/context/behaviour/EditManager.hpp>

BLOOPER_NAMESPACE_BEGIN

class EditTrackComponent::ButtonPlayhead final :
    public juce::Component,

    public juce::Timer
{
 public:
  explicit ButtonPlayhead(EditTrackComponent* parent)
      : parent(move(parent)),
        colourId()
  { }

  ~ButtonPlayhead() final
  {
    this->stopTimer();
  }


  JuceColourId colourId;


 private:
  [[maybe_unused]] EditTrackComponent* parent;


  // Component

 private:
  void paint(JuceGraphics& g) final
  {
    g.setColour(
        this->getLookAndFeel().findColour(
            colourId));

    g.drawRect(
        static_cast<int>(
            this->parent->track->getProgress() *
            this->getWidth()),
        0,
        2,
        this->getHeight());
  }


  // Timer

 private:
  void timerCallback() final
  {
    this->repaint();
  }
};


class EditTrackComponent::Button final :
    public juce::DrawableButton,

    private JuceStateListener,
    private util::FlaggedAsyncUpdater
{
 public:
  explicit Button(EditTrackComponent* parent)
      : juce::DrawableButton(
            "Track",
            juce::DrawableButton::ButtonStyle::ImageFitted),
        parent(move(parent)),

        playhead(this->parent),

        playheadUpdate(false),
        imageUpdate(false)
  {
    ext::addAndMakeVisible(
        *this,
        this->playhead);

    this->parent->track->getState().addListener(this);
    this->parent->getContext().getEditManager().addListener(this);

    this->updatePlayhead();
    this->updateImages();
  }

  ~Button() override
  {
    this->parent->getContext().getEditManager().removeListener(this);
    this->parent->track->getState().removeListener(this);
  }


 private:
  [[maybe_unused]] EditTrackComponent* parent;


  [[maybe_unused]] ButtonPlayhead playhead;


  bool playheadUpdate;

  void updatePlayhead()
  {
    auto& parentTrack = this->parent->track;
    if (parentTrack->playback == TrackPlayback::paused)
    {
      this->playhead.stopTimer();
      this->playhead.setVisible(false);
    }
    else if (parentTrack->playback == TrackPlayback::playing)
    {
      this->playhead.startTimerHz(20);
      this->playhead.colourId = ColourId::yellowBright;
      this->playhead.setVisible(true);
    }
    else if (parentTrack->playback == TrackPlayback::recording)
    {
      this->playhead.startTimerHz(20);
      this->playhead.colourId = ColourId::redBright;
      this->playhead.setVisible(true);
    }
  }


  bool imageUpdate;

  void updateImages()
  {
    auto& assets = this->parent->getContext().getAssetManager();
    auto& parentTrack = this->parent->track;
    auto  isClear = parentTrack->getAudio().getClips().size() == 0;

    const JuceDrawable* stateImage = nullptr;
    const JuceDrawable* nextStateImage = nullptr;

    if (parentTrack->playback == TrackPlayback::paused)
    {
      stateImage = assets.getIconView(assets::IconAssetId::play);

      if (isClear)
      {
        nextStateImage = assets.getIconView(assets::IconAssetId::record);
        maybeUnused(nextStateImage);
      }
      {
        nextStateImage = assets.getIconView(assets::IconAssetId::pause);
      }
    }
    else if (parentTrack->playback == TrackPlayback::playing)
    {
      stateImage = assets.getIconView(assets::IconAssetId::play);
      nextStateImage = assets.getIconView(assets::IconAssetId::pause);
    }
    else if (parentTrack->playback == TrackPlayback::recording)
    {
      stateImage = assets.getIconView(assets::IconAssetId::record);
      nextStateImage = assets.getIconView(assets::IconAssetId::del);
    }

    const JuceDrawable* _disabledImage =
        assets.getIconView(assets::IconAssetId::muteTrack);

    this->setImages(
        stateImage,
        nextStateImage,
        nextStateImage,
        _disabledImage,
        stateImage,
        nextStateImage,
        nextStateImage,
        _disabledImage);
  }


  // Button

 private:
  void clicked() final
  {
    auto& parentTrack = this->parent->track;
    auto  isClear = parentTrack->getAudio().getClips().size() == 0;

    if (parentTrack->playback == TrackPlayback::playing)
    {
      parentTrack->playback = TrackPlayback::paused;
    }
    else if (parentTrack->playback == TrackPlayback::recording)
    {
      parentTrack->playback = TrackPlayback::paused;
    }
    else if (parentTrack->playback == TrackPlayback::paused)
    {
      if (isClear)
      {
        parentTrack->playback = TrackPlayback::recording;
      }
      else
      {
        parentTrack->playback = TrackPlayback::playing;
      }
    }
  }

  void clicked(const juce::ModifierKeys& modifiers) final
  {
    auto& parentTrack = this->parent->track;

    auto withCommand = modifiers.isCommandDown();
    if (withCommand)
    {
      parentTrack->clear();
      return;
    }

    this->clicked();
  }


  // ValueTreeListener

 private:
  void valueTreeChanged() final { }

  void valueTreePropertyChanged(
      juce::ValueTree&        tree,
      const juce::Identifier& id) final
  {
    auto& parentTrack = this->parent->track;
    auto& editManager = this->parent->getContext().getEditManager();

    if (tree == parentTrack->getState())
    {
      if (id == id::playback)
      {
        this->markAndUpdate(this->imageUpdate);
        this->markAndUpdate(this->playheadUpdate);
      }
      else if (id == id::interval)
      {
      }
      else if (id == id::mode)
      {
        this->markAndUpdate(this->playheadUpdate);
      }
    }
    else if (tree == editManager.getState())
    {
      if (id == id::soloed)
      {
        this->markAndUpdate(this->imageUpdate);
      }
    }
  }


  // FlaggedAsyncUpdater

 private:
  void handleAsyncUpdate() final
  {
    if (util::FlaggedAsyncUpdater::compareAndReset(imageUpdate))
    {
      this->updateImages();
    }
  }
};


EditTrackComponent::EditTrackComponent(
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

  this->button =
      std::make_unique<EditTrackComponent::Button>(
          this);


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


  // Plugin List

  TrackPluginListComponent::Options pluginListOptions{};

  pluginList =
      std::make_unique<TrackPluginListComponent>(
          this->getContext(),
          this->getState().getOrCreateChildWithName(
              TrackPluginListComponent::stateId,
              nullptr),
          this->track,
          move(pluginListOptions));


  // Add

  ext::addAndMakeVisible(
      *this,
      *this->name,
      *this->button,

      *this->muteButton,
      *this->soloButton,
      *this->armButton,

      *this->modeDropdown,
      *this->intervalDropdown,

      *this->pluginList);


  // RAII

  this->track->getAudio().state.addListener(this);

  this->name->addListener(this);

  this->modeDropdown->addListener(this);
  this->intervalDropdown->addListener(this);
}

EditTrackComponent::~EditTrackComponent()
{
  this->intervalDropdown->removeListener(this);
  this->modeDropdown->removeListener(this);

  this->name->removeListener(this);

  this->track->getAudio().state.removeListener(this);
}


// Component

void EditTrackComponent::resized()
{
  auto availableArea = this->getLocalBounds();


  // Header

  this->name->setBounds(
      availableArea.removeFromTop(
          static_cast<int>(
              this->getLookAndFeel()
                  .getLabelFont(*this->name)
                  .getHeight()) +
          4));

  this->button->setBounds(
      availableArea.removeFromTop(
                       availableArea.getWidth())
          .reduced(2));


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

  this->muteButton->setBounds(topSmallButtonArea);

  this->armButton->setBounds(
      bottomSmallButtonArea
          .removeFromLeft(
              static_cast<int>(
                  bottomSmallButtonArea.getWidth() / 2))
          .reduced(4));


  // Dropdowns

  this->modeDropdown->setBounds(
      availableArea.removeFromTop(
          50));

  this->intervalDropdown->setBounds(
      availableArea.removeFromTop(
          50));


  // Plugin List

  this->pluginList->setBounds(availableArea);
}

void EditTrackComponent::mouseDown(const juce::MouseEvent&)
{
  this->getContext()
      .getSelectionManager()
      .select(
          this->track,
          true);
}


// ValueTreeListener

void EditTrackComponent::valueTreePropertyChanged(
    juce::ValueTree&        tree,
    const juce::Identifier& id)
{
  if (tree.hasType(te::IDs::TRACK))
  {
    if (id == te::IDs::NAME)
    {
      // TODO: this might update the text twice - fix
      //      this->name->setText(
      //          tree.getProperty(id),
      //          juce::dontSendNotification);
    }
  }
}


// LabelListener

void EditTrackComponent::labelTextChanged(
    juce::Label* labelThatHasChanged)
{
  if (this->name.get() == labelThatHasChanged)
  {
    this->track->getAudio().setName(this->name->getText());
  }
}

// ComboBoxListener

void EditTrackComponent::comboBoxChanged(
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
