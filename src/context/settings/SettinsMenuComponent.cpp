#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

SettingsMenuComponent::SettingsMenuComponent(
    AbstractCoreContext& context,
    State                state,
    Options              options)
    : CoreComponentBase(
          context,
          move(state)),
      options(move(options))
{
  this->openedTabIndex.referTo(
      this->getState(),
      SettingsMenuComponent::openedTabIndexId,
      nullptr,
      -1);


  SettingsAppearanceComponent::Options appearanceOptions{};

  this->appearance =
      std::make_unique<SettingsAppearanceComponent>(
          this->getContext(),
          this->getState().getOrCreateChildWithName(
              SettingsAppearanceComponent::stateId,
              nullptr),
          move(appearanceOptions));

  SettingsBehaviourComponent::Options behaviourOptions{};

  this->behaviour =
      std::make_unique<SettingsBehaviourComponent>(
          this->getContext(),
          this->getState().getOrCreateChildWithName(
              SettingsBehaviourComponent::stateId,
              nullptr),
          move(behaviourOptions));

  SettingsKeymapsComponent::Options keymapsOptions{};

  this->keymaps =
      std::make_unique<SettingsKeymapsComponent>(
          this->getContext(),
          this->getState().getOrCreateChildWithName(
              SettingsKeymapsComponent::stateId,
              nullptr),
          move(keymapsOptions));

  SettingsDevicesComponent::Options devicesOptions{};

  this->devices =
      std::make_unique<SettingsDevicesComponent>(
          this->getContext(),
          this->getState().getOrCreateChildWithName(
              SettingsDevicesComponent::stateId,
              nullptr),
          move(devicesOptions));

  SettingsPluginsComponent::Options pluginsOptions{};

  this->plugins =
      std::make_unique<SettingsPluginsComponent>(
          this->getContext(),
          this->getState().getOrCreateChildWithName(
              SettingsPluginsComponent::stateId,
              nullptr),
          move(pluginsOptions));

  SettingsProjectsComponent::Options projectsOptions{};

  this->projects =
      std::make_unique<SettingsProjectsComponent>(
          this->getContext(),
          this->getState().getOrCreateChildWithName(
              SettingsProjectsComponent::stateId,
              nullptr),
          move(projectsOptions));


  this->tabs =
      std::make_unique<juce::TabbedComponent>(
          juce::TabbedButtonBar::Orientation::TabsAtLeft);

  this->tabs->addTab(
      "Appearance",
      this->getLookAndFeel().findColour(
          juce::TabbedComponent::ColourIds::backgroundColourId),
      this->appearance.get(),
      false);

  this->tabs->addTab(
      "Behaviour",
      this->getLookAndFeel().findColour(
          juce::TabbedComponent::ColourIds::backgroundColourId),
      this->behaviour.get(),
      false);

  this->tabs->addTab(
      "Keymaps",
      this->getLookAndFeel().findColour(
          juce::TabbedComponent::ColourIds::backgroundColourId),
      this->keymaps.get(),
      false);

  this->tabs->addTab(
      "Devices",
      this->getLookAndFeel().findColour(
          juce::TabbedComponent::ColourIds::backgroundColourId),
      this->devices.get(),
      false);

  this->tabs->addTab(
      "Projects",
      this->getLookAndFeel().findColour(
          juce::TabbedComponent::ColourIds::backgroundColourId),
      this->projects.get(),
      false);

  this->tabs->addTab(
      "Plugins",
      this->getLookAndFeel().findColour(
          juce::TabbedComponent::ColourIds::backgroundColourId),
      this->plugins.get(),
      false);

  if (this->openedTabIndex >= 0)
  {
    this->tabs->setCurrentTabIndex(
        this->openedTabIndex,
        false);
  }


  ext::addAndMakeVisible(
      *this,
      *this->tabs);
}

SettingsMenuComponent::~SettingsMenuComponent()
{
  this->openedTabIndex = this->tabs->getCurrentTabIndex();
}


// Component

[[maybe_unused]] void SettingsMenuComponent::resized()
{
  auto availableArea = this->getLocalBounds();

  this->tabs->setBounds(availableArea);
}

BLOOPER_NAMESPACE_END
