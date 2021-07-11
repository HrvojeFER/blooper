#ifndef BLOOPER_COMMAND_STATUS_COMPONENT_HPP
#define BLOOPER_COMMAND_STATUS_COMPONENT_HPP

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

struct CommandStatusOptions
{
  bool shouldResizeItself = true;
  bool shouldResizeParentWhenResized = true;
};

class CommandStatusComponent final :
    public ComponentBase
{
 public:
  BLOOPER_STATE_ID(CommandStatusComponent);


  explicit CommandStatusComponent(
      AbstractContext&     context,
      State                state,
      CommandStatusOptions options = {});

  ~CommandStatusComponent() override;

  CommandStatusOptions options;


 private:
  JuceCached<JuceCommandId> lastCommandId;

  std::unique_ptr<juce::Label> label;


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


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CommandStatusComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_COMMAND_STATUS_COMPONENT_HPP
