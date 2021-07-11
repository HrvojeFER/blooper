#ifndef BLOOPER_CPU_STATUS_COMPONENT_HPP
#define BLOOPER_CPU_STATUS_COMPONENT_HPP

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

struct CPUStatusOptions
{
  bool shouldResizeItself = true;
  bool shouldResizeParentWhenResized = true;
};

class CPUStatusComponent final :
    public ComponentBase,

    private te::DeviceManager::CPUUsageListener,
    private JuceTimer
{
 public:
  BLOOPER_STATE_ID(CPUStatusComponent);


  explicit CPUStatusComponent(
      AbstractContext& context,
      State            state,
      CPUStatusOptions options = {});

  ~CPUStatusComponent() override;

  CPUStatusOptions options;


 private:
  juce::Atomic<int> cpuAvg, cpuMin, cpuMax, numGlitches;

  std::unique_ptr<juce::Label> label;


  void updateLabel();

  void resizeLabel();


  // Component

 public:
  void resized() override;


  // Timer

 private:
  void timerCallback() override;


  // CPUUsageListener

 private:
  // reminder: this is on the audio thread!
  void reportCPUUsage(
      float cpuAvg,
      float cpuMin,
      float cpuMax,
      int   numGlitches) override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CPUStatusComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_CPU_STATUS_COMPONENT_HPP
