#ifndef BLOOPER_ENGINE_BEHAVIOUR_HPP
#define BLOOPER_ENGINE_BEHAVIOUR_HPP


#include <blooper/abstract.hpp>


BLOOPER_NAMESPACE_BEGIN

class EngineBehaviour : public te::EngineBehaviour
{
public:
    EngineBehaviour(AbstractCoreContext& context);
    ~EngineBehaviour() override;


private:
    AbstractCoreContext& context;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EngineBehaviour)
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_ENGINE_BEHAVIOUR_HPP
