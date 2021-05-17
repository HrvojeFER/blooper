#ifndef BLOOPER_PROJECT_COMPONENT_HPP
#define BLOOPER_PROJECT_COMPONENT_HPP


#include <blooper/fwd.hpp>

#include <blooper/context/core/core.hpp>


BLOOPER_NAMESPACE_BEGIN

class ProjectComponent : public CoreComponent
{
public:
    explicit ProjectComponent(CoreContext& context);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectComponent);
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_PROJECT_COMPONENT_HPP
