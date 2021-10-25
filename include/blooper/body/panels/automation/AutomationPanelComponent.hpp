#ifndef BLOOPER_AUTOMATION_PANEL_COMPONENT_HPP
#define BLOOPER_AUTOMATION_PANEL_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

class AutomationPanelComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(AutomationPanelComponent);


  struct Options
  {
  } options;

  explicit AutomationPanelComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});

  ~AutomationPanelComponent() override;


  // Component

 public:
  void resized() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AutomationPanelComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_AUTOMATION_PANEL_COMPONENT_HPP
