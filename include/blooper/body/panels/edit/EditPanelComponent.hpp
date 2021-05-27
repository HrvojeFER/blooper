#ifndef BLOOPER_EDIT_PANEL_COMPONENT_HPP
#define BLOOPER_EDIT_PANEL_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class EditPanelComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(EditPanelComponent);


  struct Options
  {
  } options;

  explicit EditPanelComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});


  void resized() override;


 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EditPanelComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_EDIT_PANEL_COMPONENT_HPP
