#include <blooper/context/settings/SettingsBehaviourComponent.hpp>

#include <blooper/components/properties/FilePathPropertyComponent.hpp>

#include <blooper/internal/abstract/id.hpp>
#include <blooper/internal/ext/component.hpp>
#include <blooper/internal/ext/value_tree.hpp>

BLOOPER_NAMESPACE_BEGIN

SettingsBehaviourComponent::SettingsBehaviourComponent(
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
      SettingsBehaviourComponent::panelOpennessStateId,
      nullptr,
      "");


  this->settingsRoot =
      this->getContext()
          .getSettings()
          .getOrCreateChildWithName(
              id::behaviour,
              nullptr);


  const auto defaultRootBrowserFolder =
      juce::File::getSpecialLocation(juce::File::userHomeDirectory)
          .getFullPathName();

  ext::referTo(
      this->rootBrowserFolder,
      this->settingsRoot,
      id::rootBrowserFolder,
      undoManager,
      defaultRootBrowserFolder);


  this->panel = std::make_unique<juce::PropertyPanel>("Behaviour");

  this->panel->addSection(
      "Browser",
      {new FilePathPropertyComponent(
          this->rootBrowserFolder.getPropertyAsValue(),
          "Root folder",
          {true,
           true,
           "*",
           defaultRootBrowserFolder})},
      true,
      -1,
      2);

  this->panel->addSection(
      "Tracks",
      {},
      true,
      -1,
      2);

  this->panel->addSection(
      "Master Track",
      {},
      true,
      -1,
      2);

  this->panel->addSection(
      "Control surface",
      {},
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

SettingsBehaviourComponent::~SettingsBehaviourComponent()
{
  {
    auto xml = this->panel->getOpennessState();
    this->panelOpennessState = xml->toString();
  }
}


// Component

[[maybe_unused]] void SettingsBehaviourComponent::resized()
{
  auto availableArea = this->getLocalBounds();

  this->panel->setBounds(availableArea);
}

BLOOPER_NAMESPACE_END
