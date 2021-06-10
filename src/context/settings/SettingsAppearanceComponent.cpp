#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

SettingsAppearanceComponent::SettingsAppearanceComponent(
    AbstractCoreContext& context,
    State                state,
    Options              options)
    : CoreComponentBase(
          context,
          move(state)),
      options(move(options))
{
  this->panelOpennessState.referTo(
      this->getState(),
      SettingsAppearanceComponent::panelOpennessStateId,
      nullptr,
      "");

  this->settingsRoot =
      this->getContext()
          .getSettings()
          .getOrCreateChildWithName(
              id::appearance,
              nullptr);


  this->panel = std::make_unique<juce::PropertyPanel>("Appearance");

  auto undoManager =
      std::addressof(
          this->getContext().getUndoManager());

  this->panel->addSection(
      "Panels",
      {new juce::SliderPropertyComponent(
           settingsRoot.getPropertyAsValue(
               id::masterTrackPanelSize,
               undoManager),
           "Master Track Panel size",
           50,
           500,
           1),
       new juce::SliderPropertyComponent(
           settingsRoot.getPropertyAsValue(
               id::controlSurfacePanelSize,
               undoManager),
           "Control surface Panel size",
           50,
           500,
           1),
       new juce::SliderPropertyComponent(
           settingsRoot.getPropertyAsValue(
               id::browserPanelSize,
               undoManager),
           "Browser Panel size",
           50,
           500,
           1)},
      true,
      -1,
      2);

  this->panel->addSection(
      "Tracks",
      {new juce::SliderPropertyComponent(
          settingsRoot.getPropertyAsValue(
              id::trackSize,
              undoManager),
          "Track size",
          50,
          500,
          1)},
      true,
      -1,
      2);

  if (this->panelOpennessState->isNotEmpty())
  {
    auto doc = JuceXmlDoc{this->panelOpennessState};
    auto xml = doc.getDocumentElement();
    this->panel->restoreOpennessState(*xml);
  }
}

SettingsAppearanceComponent::~SettingsAppearanceComponent()
{
  {
    auto xml = this->panel->getOpennessState();
    this->panelOpennessState = xml->toString();
  }
}


// Component

[[maybe_unused]] void SettingsAppearanceComponent::resized()
{
  auto availableArea = this->getLocalBounds();

  this->panel->setBounds(availableArea);
}

BLOOPER_NAMESPACE_END
