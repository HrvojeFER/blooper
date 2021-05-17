#ifndef BLOOPER_CORE_CONTEXT_HPP
#define BLOOPER_CORE_CONTEXT_HPP


#include <blooper/fwd.hpp>


BLOOPER_NAMESPACE_BEGIN

class CoreContext
{
public:
    CoreContext();

    virtual ~CoreContext();


    [[maybe_unused, nodiscard]] inline const te::Engine&
    getEngine() const noexcept;

    [[maybe_unused, nodiscard]] inline te::Engine&
    getEngine() noexcept;


    [[maybe_unused, nodiscard]] inline const juce::LookAndFeel&
    getLookAndFeel() const noexcept;

    [[maybe_unused, nodiscard]] inline juce::LookAndFeel&
    getLookAndFeel() noexcept;


protected:
    std::unique_ptr<te::Engine> engine;

    std::unique_ptr<juce::LookAndFeel> lookAndFeel;


private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CoreContext)
};


const te::Engine& CoreContext::getEngine() const noexcept
{
    return *engine;
}

te::Engine& CoreContext::getEngine() noexcept
{
    return *engine;
}


const juce::LookAndFeel& CoreContext::getLookAndFeel() const noexcept
{
    return *lookAndFeel;
}

juce::LookAndFeel& CoreContext::getLookAndFeel() noexcept
{
    return *lookAndFeel;
}


BLOOPER_NAMESPACE_END


#endif // BLOOPER_CORE_CONTEXT_HPP
