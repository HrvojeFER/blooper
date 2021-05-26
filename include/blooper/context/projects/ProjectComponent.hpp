#ifndef BLOOPER_PROJECT_COMPONENT_HPP
#define BLOOPER_PROJECT_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class ProjectComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(ProjectComponent);


  class Options
  {
  } options;

  explicit ProjectComponent(
      AbstractContext& context,
      State            state,
      JuceProjectRef   project = {},
      Options          options = {});


 private:
  JuceProjectRef project;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectComponent);
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PROJECT_COMPONENT_HPP
