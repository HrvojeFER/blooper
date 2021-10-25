#include <blooper/context/projects/ProjectComponent.hpp>

#include <blooper/internal/abstract/id.hpp>
#include <blooper/internal/ext/component.hpp>
#include <blooper/internal/ext/value_tree.hpp>

BLOOPER_NAMESPACE_BEGIN

ProjectComponent::ProjectComponent(
    AbstractContext& context,
    State            state,
    JuceProjectRef   project,
    Options          options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options)),
      project(move(project))
{
  auto undoManager = this->getContext().getUndoManagerPtr();


  this->panelOpennessState.referTo(
      this->getState(),
      ProjectComponent::panelOpennessStateId,
      nullptr,
      "");


  this->name =
      std::make_unique<juce::Label>(
          "Project Name",
          this->getContext().getProject().getName());

  this->name->onTextChange = [this] {
    this->getContext().getProject().setName(
        this->name->getText());
  };

  this->name->setEditable(
      false,
      true);


  this->description =
      std::make_unique<juce::TextEditor>(
          "Project Description");

  this->description->setText(
      this->getContext().getProject().getDescription(),
      false);

  this->description->setMultiLine(true);

  this->description->onTextChange = [this] {
    this->getContext().getProject().setDescription(
        this->description->getText());
  };


  ext::referTo(
      this->bpm,
      this->getContext().getProjectSettings(),
      id::bpm,
      undoManager,
      120);

  this->panel = std::make_unique<juce::PropertyPanel>("Project");

  this->panel->addSection(
      "Time",
      {new juce::SliderPropertyComponent(
          this->bpm.getPropertyAsValue(),
          "Bpm",
          te::TempoSetting::minBPM,
          te::TempoSetting::maxBPM,
          0.01)});


  ext::addAndMakeVisible(
      *this,
      *this->name,
      *this->description,
      *this->panel);


  if (this->panelOpennessState->isNotEmpty())
  {
    auto doc = JuceXmlDoc{this->panelOpennessState};
    auto xml = doc.getDocumentElement();
    this->panel->restoreOpennessState(*xml);
  }
}

ProjectComponent::~ProjectComponent()
{
  {
    auto xml = this->panel->getOpennessState();
    this->panelOpennessState = xml->toString();
  }
}


// Component

void ProjectComponent::resized()
{
  auto availableArea = this->getLocalBounds();


  const auto nameHeight =
      static_cast<int>(
          this->getLookAndFeel()
              .getLabelFont(*this->name)
              .getHeight() +
          10);

  this->name->setBounds(
      availableArea
          .removeFromTop(nameHeight)
          .reduced(2));

  this->description->setBounds(
      availableArea
          .removeFromTop(nameHeight * 5)
          .reduced(2));


  this->panel->setBounds(availableArea);
}

BLOOPER_NAMESPACE_END
