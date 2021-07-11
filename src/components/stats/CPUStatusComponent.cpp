#include <blooper/components/stats/CPUStatusComponent.hpp>

#include <blooper/internal/ext/component.hpp>
#include <blooper/internal/utils/style.hpp>

BLOOPER_NAMESPACE_BEGIN

CPUStatusComponent::CPUStatusComponent(
    AbstractContext& context,
    State            state,
    CPUStatusOptions options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options)),

      cpuAvg(0),
      cpuMin(0),
      cpuMax(0),
      numGlitches(0)
{
  this->label = addJuceChild<juce::Label>(*this, "CPU Usage");

  this->updateLabel();

  this->getContext()
      .getEngine()
      .getDeviceManager()
      .addCPUUsageListener(this);

  this->startTimerHz(20);
}

CPUStatusComponent::~CPUStatusComponent()
{
  this->stopTimer();

  this->getContext()
      .getEngine()
      .getDeviceManager()
      .removeCPUUsageListener(this);
}


void CPUStatusComponent::updateLabel()
{
  const auto _cpuAvgString =
      JuceString{
          this->cpuAvg.value.load(
              std::memory_order_relaxed) /
              1000.0,
          3};

  const auto _cpuMinString =
      JuceString{
          this->cpuMin.value.load(
              std::memory_order_relaxed) /
              1000.0,
          3};

  const auto _cpuMaxString =
      JuceString{
          this->cpuMax.value.load(
              std::memory_order_relaxed) /
              1000.0,
          3};

  const auto _numGlitches =
      this->numGlitches.value.load(
          std::memory_order_relaxed);

  const auto _numGlitchesString =
      JuceString{_numGlitches};


  if (_numGlitches)
  {
    this->label->setColour(
        juce::Label::textColourId,
        this->findColour(ColourId::red));
  }
  else
  {
    this->label->setColour(
        juce::Label::textColourId,
        this->findColour(juce::Label::textColourId));
  }

  this->label->setText(
      "CPU Usage: " + _cpuAvgString,
      juce::dontSendNotification);

  this->label->setTooltip(
      "CPU Average: " + _cpuAvgString + "\n" +
      "CPU Min: " + _cpuMinString + "\n" +
      "CPU Max: " + _cpuMaxString + "\n" +
      "Glitch Count: " + _numGlitchesString);


  if (this->options.shouldResizeItself)
    this->resizeLabel();
}

void CPUStatusComponent::resizeLabel()
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

void CPUStatusComponent::resized()
{
  auto availableArea = this->getLocalBounds();

  this->label->setBounds(availableArea);
}


// CPUUsageListener

// reminder: this is on the audio thread!
void CPUStatusComponent::reportCPUUsage(
    float _cpuAvg,
    float _cpuMin,
    float _cpuMax,
    int   _numGlitches)
{
  this->cpuAvg.value.store(
      static_cast<int>(_cpuAvg * 1000),
      std::memory_order_relaxed);

  this->cpuMin.value.store(
      static_cast<int>(_cpuMin * 1000),
      std::memory_order_relaxed);

  this->cpuMax.value.store(
      static_cast<int>(_cpuMax * 1000),
      std::memory_order_relaxed);

  this->numGlitches.value.store(
      _numGlitches,
      std::memory_order_relaxed);
}


// Timer

void CPUStatusComponent::timerCallback()
{
  this->updateLabel();
}

BLOOPER_NAMESPACE_END
