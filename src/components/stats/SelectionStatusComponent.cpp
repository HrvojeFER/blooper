#include <blooper/components/stats/SelectionStatusComponent.hpp>

#include <blooper/internal/ext/component.hpp>

BLOOPER_NAMESPACE_BEGIN

SelectionStatusComponent::SelectionStatusComponent(
    AbstractContext&       context,
    State                  state,
    SelectionStatusOptions options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options)),

      selection(nullptr)
{
  this->label = addJuceChild<juce::Label>(*this, "Selection Status");

  this->updateSelection();

  this->selection->addChangeListener(this);
}

SelectionStatusComponent::~SelectionStatusComponent()
{
  this->selection->removeChangeListener(this);
}


void SelectionStatusComponent::updateSelection()
{
  if (this->selection ==
      this->getContext().getFocusedSelectionManager())
    return;


  if (this->selection)
    this->selection->removeChangeListener(this);


  this->selection = this->getContext().getFocusedSelectionManager();

  if (this->selection)
    this->selection->addChangeListener(this);


  this->updateLabel();
}

void SelectionStatusComponent::updateLabel()
{
  // TODO: tooltip

  if (!this->selection)
  {
    this->label->setText(
        "Nothing to select",
        juce::dontSendNotification);
  }
  else if (auto selectableClass =
               this->selection->getFirstSelectableClass())
  {
    this->label->setText(
        "Current selection: " +
            selectableClass->getDescriptionOfSelectedGroup(
                this->selection->getSelectedObjects()),
        juce::dontSendNotification);
  }
  else
  {
    this->label->setText(
        "Nothing selected",
        juce::dontSendNotification);
  }

  if (this->options.shouldResizeItself)
    this->resizeLabel();
}

void SelectionStatusComponent::resizeLabel()
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

void SelectionStatusComponent::resized()
{
  auto availableArea = this->getLocalBounds();

  this->label->setBounds(availableArea);
}


// ChangeListener

void SelectionStatusComponent::changeListenerCallback(
    juce::ChangeBroadcaster* source)
{
  if (source == std::addressof(this->getContext()))
  {
    this->updateSelection();
  }
  else if (source == this->selection)
  {
    this->updateLabel();
  }
}

BLOOPER_NAMESPACE_END
