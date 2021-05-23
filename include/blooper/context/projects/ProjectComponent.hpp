#ifndef BLOOPER_PROJECT_COMPONENT_HPP
#define BLOOPER_PROJECT_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class ProjectComponent : public ComponentBase
{
 public:
  explicit ProjectComponent(AbstractContext& context);

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectComponent);
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PROJECT_COMPONENT_HPP
