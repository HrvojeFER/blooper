#ifndef BLOOPER_TRANSPORT_STATUS_COMPONENT_HPP
#define BLOOPER_TRANSPORT_STATUS_COMPONENT_HPP

#include <blooper/internal/abstract/components.hpp>
#include <blooper/internal/utils/FlaggedAsyncUpdater.hpp>

BLOOPER_NAMESPACE_BEGIN

struct TransportStatusOptions
{
  bool shouldResizeItself = true;
  bool shouldResizeParentWhenResized = true;
};

class TransportStatusComponent final :
    public ComponentBase,

    private juce::Timer
{
 public:
  BLOOPER_STATE_ID(TransportStatusComponent);


  explicit TransportStatusComponent(
      AbstractContext&       context,
      State                  state,
      TransportStatusOptions options = {});

  ~TransportStatusComponent() override;

  TransportStatusOptions options;


 private:
  JuceTransportRef transport;

  std::unique_ptr<juce::Label> label;


  void updateFocus();


  void updateLabel();

  void resizeLabel();


  // Component

 public:
  void resized() override;


  // ValueTreeListener

 private:
  void valueTreePropertyChanged(
      juce::ValueTree&        tree,
      const juce::Identifier& identifier) override;


  // Timer

 private:
  void timerCallback() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TransportStatusComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_TRANSPORT_STATUS_COMPONENT_HPP
