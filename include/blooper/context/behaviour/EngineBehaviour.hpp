#ifndef BLOOPER_ENGINE_BEHAVIOUR_HPP
#define BLOOPER_ENGINE_BEHAVIOUR_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class EngineBehaviour :
    public CoreContextualBase,
    public te::EngineBehaviour
{
 public:
  explicit EngineBehaviour(AbstractCoreContext& context);


 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EngineBehaviour)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_ENGINE_BEHAVIOUR_HPP
