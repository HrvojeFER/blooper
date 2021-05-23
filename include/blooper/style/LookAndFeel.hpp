#ifndef BLOOPER_LOOK_AND_FEEL_HPP
#define BLOOPER_LOOK_AND_FEEL_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class LookAndFeel :
    public CoreContextualBase,
    public juce::LookAndFeel_V4
{
 public:
  explicit LookAndFeel(AbstractCoreContext& context);


 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LookAndFeel)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_LOOK_AND_FEEL_HPP
