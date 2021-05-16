#ifndef BLOOPER_ENGINE_BEHAVIOUR_HPP
#define BLOOPER_ENGINE_BEHAVIOUR_HPP


BLOOPER_NAMESPACE_BEGIN

class CoreContext;


class EngineBehaviour : public te::EngineBehaviour
{
public:
    EngineBehaviour(CoreContext& context);
    ~EngineBehaviour() override;


private:
    CoreContext& context;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EngineBehaviour)
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_ENGINE_BEHAVIOUR_HPP
