#ifndef BLOOPER_PROJECT_COMPONENT_HPP
#define BLOOPER_PROJECT_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class ProjectComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(ProjectComponent);

  BLOOPER_ID(panelOpennessStateId);


  class Options
  {
  } options;

  explicit ProjectComponent(
      AbstractContext& context,
      State            state,
      JuceProjectRef   project = {},
      Options          options = {});

  ~ProjectComponent() override;


 private:
  JuceCached<JuceString> panelOpennessState;

  JuceProjectRef project;


  JuceCached<double> bpm;

  std::unique_ptr<juce::PropertyPanel> panel;


  std::unique_ptr<juce::Label> name;

  std::unique_ptr<juce::TextEditor> description;


  // Component

 public:
  void resized() override;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectComponent);
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PROJECT_COMPONENT_HPP
