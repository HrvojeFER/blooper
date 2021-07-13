#include <blooper/components/stats/TransportStatusComponent.hpp>

#include <blooper/internal/abstract/id.hpp>
#include <blooper/internal/ext/component.hpp>
#include <blooper/internal/ext/edit.hpp>
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
      options(move(options)),

      transport(nullptr)
{
  this->label = addJuceChild<juce::Label>(*this, "Transport Status");

  this->updateFocus();


  this->getContext().getEditManager().addListener(this);

  this->startTimerHz(defaultGuiTimerHz);
}

TransportStatusComponent::~TransportStatusComponent()
{
  this->stopTimer();

  this->getContext().getEditManager().removeListener(this);
}


void TransportStatusComponent::updateFocus()
{
  this->transport =
      std::addressof(
          this->getContext()
              .getFocusedEdit()
              ->getTransport());

  this->updateLabel();
}

void TransportStatusComponent::updateLabel()
{
  JuceColourId textColourId;
  JuceString   transportStatus;

  if (!this->transport)
  {
    textColourId = juce::Label::textColourId;
    transportStatus = "No Edit in focus";
  }
  else if (this->transport->isPlaying())
  {
    if (this->transport->isRecording())
    {
      textColourId = ColourId::red;
      transportStatus =
          "Recording at: " +
          JuceString{
              ext::getPositionBeats(
                  *this->transport),
              3};
    }
    else
    {
      textColourId = ColourId::green;
      transportStatus =
          "Playing at: " +
          JuceString{
              ext::getPositionBeats(
                  *this->transport),
              3};
    }
  }
  else
  {
    textColourId = ColourId::blue;
    transportStatus =
        "Paused at: " +
        JuceString{
            ext::getPositionBeats(
                *this->transport),
            3};
  }


  this->label->setColour(
      juce::Label::textColourId,
      this->findColour(textColourId));

  this->label->setText(
      transportStatus,
      juce::dontSendNotification);

  this->label->setTooltip(
      transportStatus +
      "\n" +
      "Master playing at: " +
      JuceString{
          ext::getPositionBeats(
              this->getContext()
                  .getEditManager()
                  .getMasterEdit()),
          3});


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
  if (tree == this->getContext().getEditManager().getState())
  {
    if (identifier == id::focusedEdit)
    {
      this->updateFocus();
    }
  }
}


// Timer

void TransportStatusComponent::timerCallback()
{
  this->updateLabel();
}

BLOOPER_NAMESPACE_END
