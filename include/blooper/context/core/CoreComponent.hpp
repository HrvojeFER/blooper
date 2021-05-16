#ifndef BLOOPER_CORE_COMPONENT_HPP
#define BLOOPER_CORE_COMPONENT_HPP


BLOOPER_NAMESPACE_BEGIN

class CoreContext;

class CoreComponent : public juce::Component
{
public:
    CoreComponent(CoreContext& context);
    ~CoreComponent() override;

private:
    CoreContext& context;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CoreComponent);
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_CORE_COMPONENT_HPP
