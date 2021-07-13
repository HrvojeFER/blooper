#include <blooper/components/stats/CommandStatusComponent.hpp>

#include <blooper/internal/abstract/id.hpp>
#include <blooper/internal/ext/component.hpp>
#include <blooper/internal/utils/ContextCommands.hpp>

BLOOPER_NAMESPACE_BEGIN

CommandStatusComponent::CommandStatusComponent(
    AbstractContext&     context,
    State                state,
    CommandStatusOptions options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
  this->lastCommandId.referTo(
      this->getContext().getState(),
      id::lastCommandId,
      nullptr,
      CommandId::none);


  this->label =
      std::make_unique<juce::Label>(
          "Command Status");


  ext::addAndMakeVisible(
      *this,
      *this->label);

  this->updateLabel();


  this->getContext().getState().addListener(this);
}

CommandStatusComponent::~CommandStatusComponent()
{
  this->getContext().getState().removeListener(this);
}


void CommandStatusComponent::updateLabel()
{
  this->lastCommandId.forceUpdateOfCachedValue();
  if (this->lastCommandId == CommandId::none)
  {
    this->label->setText(
        "No commands yet!",
        juce::dontSendNotification);

    this->label->setTooltip(
        "You didn't invoke any commands yet.");
  }
  else
  {
    const auto info = getCommandInfo(this->lastCommandId);

    this->label->setText(
        "Last command: " + info.shortName,
        juce::dontSendNotification);

    this->label->setTooltip(
        "Category: " + info.categoryName +
        "\n" +
        "Name: " + info.shortName +
        "\n"
        "Description: \n" +
        info.description);
  }

  if (this->options.shouldResizeItself)
    this->resizeLabel();
}

void CommandStatusComponent::resizeLabel()
{
  ext::setWidth(
      *this,
      this->getLookAndFeel()
          .getLabelFont(*this->label)
          .getStringWidth(this->label->getText()));

  if (this->options.shouldResizeParentWhenResized)
    if (auto parent = this->getParentComponent())
      parent->resized();
}


// Component

void CommandStatusComponent::resized()
{
  auto availableArea = this->getLocalBounds();

  this->label->setBounds(availableArea);
}


// ValueTreeListener

void CommandStatusComponent::valueTreePropertyChanged(
    juce::ValueTree&        tree,
    const juce::Identifier& identifier)
{
  if (tree == this->getContext().getState())
  {
    if (identifier == id::lastCommandId)
    {
      this->updateLabel();
    }
  }
}

BLOOPER_NAMESPACE_END
