#ifndef BLOOPER_SELECTION_STATUS_COMPONENT_HPP
#define BLOOPER_SELECTION_STATUS_COMPONENT_HPP

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

struct SelectionStatusOptions
{
  bool shouldResizeItself = true;
  bool shouldResizeParentWhenResized = true;
};

class SelectionStatusComponent final :
    public ComponentBase,

    private JuceChangeListener
{
 public:
  BLOOPER_STATE_ID(SelectionStatusComponent);


  explicit SelectionStatusComponent(
      AbstractContext&       context,
      State                  state,
      SelectionStatusOptions options = {});

  ~SelectionStatusComponent() override;

  SelectionStatusOptions options;


 private:
  JuceSelectionManagerRef selection;

  std::unique_ptr<juce::Label> label;


  void updateSelection();


  void updateLabel();

  void resizeLabel();


  // Component

 public:
  void resized() override;


  // ChangeListener

 private:
  void changeListenerCallback(juce::ChangeBroadcaster* source) override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SelectionStatusComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_SELECTION_STATUS_COMPONENT_HPP
