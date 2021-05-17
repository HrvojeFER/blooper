#ifndef BLOOPER_CORE_COMPONENT_HPP
#define BLOOPER_CORE_COMPONENT_HPP


#include <blooper/fwd.hpp>


BLOOPER_NAMESPACE_BEGIN

class CoreComponent : public juce::Component
{
public:
    explicit CoreComponent(CoreContext& context);
    ~CoreComponent() override;


protected:
    CoreContext& context;


private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CoreComponent);
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_CORE_COMPONENT_HPP
