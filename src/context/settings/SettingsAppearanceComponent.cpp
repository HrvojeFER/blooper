#include <blooper/context/settings/SettingsAppearanceComponent.hpp>

#include <blooper/internal/abstract/id.hpp>
#include <blooper/internal/ext/component.hpp>
#include <blooper/internal/ext/value_tree.hpp>

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
  auto undoManager =
      std::addressof(
          this->getContext().getUndoManager());


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


  ext::referTo(
      this->masterTrackPanelSize,
      this->settingsRoot,
      id::masterTrackPanelSize,
      undoManager,
      100);

  ext::referTo(
      this->controlSurfacePanelSize,
      this->settingsRoot,
      id::controlSurfacePanelSize,
      undoManager,
      100);

  ext::referTo(
      this->browserPanelSize,
      this->settingsRoot,
      id::browserPanelSize,
      undoManager,
      100);

  ext::referTo(
      this->trackSize,
      this->settingsRoot,
      id::trackSize,
      undoManager,
      100);

  ext::referTo(
      this->trackClipsSize,
      this->settingsRoot,
      id::trackClipsSize,
      undoManager,
      100);

  ext::referTo(
      this->clipSize,
      this->settingsRoot,
      id::clipSize,
      undoManager,
      50);


  this->panel = std::make_unique<juce::PropertyPanel>("Appearance");

  this->panel->addSection(
      "Panels",
      {new juce::SliderPropertyComponent(
           this->masterTrackPanelSize.getPropertyAsValue(),
           "Master Track Panel size",
           50,
           500,
           1),
       new juce::SliderPropertyComponent(
           this->controlSurfacePanelSize.getPropertyAsValue(),
           "Control surface Panel size",
           50,
           500,
           1),
       new juce::SliderPropertyComponent(
           this->browserPanelSize.getPropertyAsValue(),
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
           this->trackSize.getPropertyAsValue(),
           "Track size",
           50,
           500,
           1),
       new juce::SliderPropertyComponent(
           this->trackClipsSize.getPropertyAsValue(),
           "Track clips size",
           50,
           500,
           1),
       new juce::SliderPropertyComponent(
           this->clipSize.getPropertyAsValue(),
           "Clip size",
           20,
           200,
           1)},
      true,
      -1,
      2);


  ext::addAndMakeVisible(
      *this,
      *this->panel);


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
