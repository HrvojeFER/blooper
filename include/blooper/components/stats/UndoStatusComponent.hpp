#ifndef BLOOPER_UNDO_STATUS_COMPONENT_HPP
#define BLOOPER_UNDO_STATUS_COMPONENT_HPP

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

struct UndoStatusOptions
{
  bool shouldResizeItself = true;
  bool shouldResizeParentWhenResized = true;
};

class UndoStatusComponent final :
    public ComponentBase,

    private JuceChangeListener
{
 public:
  BLOOPER_STATE_ID(UndoStatusComponent);


  explicit UndoStatusComponent(
      AbstractContext&  context,
      State             state,
      UndoStatusOptions options = {});

  ~UndoStatusComponent() override;

  UndoStatusOptions options;


 private:
  JuceUndoManagerRef undo;

  std::unique_ptr<juce::Label> label;


  void updateUndo();


  void updateLabel();

  void resizeLabel();


  // Component

 public:
  void resized() override;


  // ChangeListener

 private:
  void changeListenerCallback(JuceChangeBroadcaster* source) override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UndoStatusComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_UNDO_STATUS_COMPONENT_HPP
