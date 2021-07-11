#include <blooper/components/stats/TransportStatusComponent.hpp>

#include <blooper/internal/ext/component.hpp>
#include <blooper/internal/utils/style.hpp>

#include <blooper/context/behaviour/EditManager.hpp>

BLOOPER_NAMESPACE_BEGIN

TransportStatusComponent::TransportStatusComponent(
    AbstractContext&       context,
    State                  state,
    TransportStatusOptions options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
  auto& masterTransport =
      this->getContext()
          .getEditManager()
          .getMasterTransport();


  this->playing.referTo(
      masterTransport.state,
      te::IDs::playing,
      nullptr,
      false);


  this->label = addJuceChild<juce::Label>(*this, "Transport Status");

  this->updateLabel();


  masterTransport.state.addListener(this);
}

TransportStatusComponent::~TransportStatusComponent()
{
  auto& masterTransport =
      this->getContext()
          .getEditManager()
          .getMasterTransport();


  masterTransport.state.removeListener(this);
}


void TransportStatusComponent::updateLabel()
{
  if (this->playing)
  {
    this->label->setColour(
        juce::Label::textColourId,
        this->findColour(ColourId::green));

    this->label->setText(
        "Playing",
        juce::dontSendNotification);
  }
  else
  {
    this->label->setColour(
        juce::Label::textColourId,
        this->findColour(ColourId::blue));

    this->label->setText(
        "Paused",
        juce::dontSendNotification);
  }


  if (this->options.shouldResizeItself)
    this->resizeLabel();
}

void TransportStatusComponent::resizeLabel()
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

void TransportStatusComponent::resized()
{
  auto availableArea = this->getLocalBounds();

  this->label->setBounds(availableArea);
}


// ValueTreeListener

void TransportStatusComponent::valueTreePropertyChanged(
    juce::ValueTree&        tree,
    const juce::Identifier& identifier)
{
  auto& masterTransport =
      this->getContext()
          .getEditManager()
          .getMasterTransport();


  if (tree == masterTransport.state)
  {
    if (identifier == te::IDs::playing)
    {
      this->playing.forceUpdateOfCachedValue();
      this->updateLabel();
    }
  }
}

BLOOPER_NAMESPACE_END
