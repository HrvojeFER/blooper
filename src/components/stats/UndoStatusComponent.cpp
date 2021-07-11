#include <blooper/components/stats/UndoStatusComponent.hpp>

#include <blooper/internal/ext/component.hpp>

BLOOPER_NAMESPACE_BEGIN

UndoStatusComponent::UndoStatusComponent(
    AbstractContext&  context,
    State             state,
    UndoStatusOptions options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options)),

      undo(nullptr)
{
  this->label = addJuceChild<juce::Label>(*this, "Undo Status");

  this->updateUndo();

  this->getContext().addChangeListener(this);
}

UndoStatusComponent::~UndoStatusComponent()
{
  this->getContext().removeChangeListener(this);
}


void UndoStatusComponent::updateUndo()
{
  if (this->undo ==
      this->getContext().getFocusedUndoManager())
    return;


  if (this->undo)
    this->undo->removeChangeListener(this);


  this->undo = this->getContext().getFocusedUndoManager();

  if (this->undo)
    this->undo->addChangeListener(this);


  this->updateLabel();
}

void UndoStatusComponent::updateLabel()
{
  if (!this->undo)
  {
    this->label->setText(
        "No undoable actions to take",
        juce::dontSendNotification);
  }
  else if (auto action = this->undo->getUndoDescription();
           action.isNotEmpty())
  {
    this->label->setText(
        "Action to undo: " + action,
        juce::dontSendNotification);
  }
  else
  {
    this->label->setText(
        "No undoable actions taken",
        juce::dontSendNotification);
  }

  if (this->options.shouldResizeItself)
    this->resizeLabel();
}

void UndoStatusComponent::resizeLabel()
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

void UndoStatusComponent::resized()
{
  auto availableArea = this->getLocalBounds();

  this->label->setBounds(availableArea);
}


// ChangeListener

void UndoStatusComponent::changeListenerCallback(
    JuceChangeBroadcaster* source)
{
  if (source == std::addressof(this->getContext()))
  {
    this->updateUndo();
  }
  else if (source == this->undo)
  {
    this->updateLabel();
  }
}

BLOOPER_NAMESPACE_END
