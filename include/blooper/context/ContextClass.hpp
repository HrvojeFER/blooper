#ifndef BLOOPER_CONTEXT_CLASS_HPP
#define BLOOPER_CONTEXT_CLASS_HPP


#include <blooper/context/EngineBehaviour.hpp>
#include <blooper/context/PropertyStorage.hpp>
#include <blooper/context/UIBehaviour.hpp>


BLOOPER_NAMESPACE_BEGIN

class Context
{
public:
    Context();
    virtual ~Context();


    [[maybe_unused, nodiscard]] const te::Engine& getEngine() const noexcept
    {
        return *engine;
    }

    [[maybe_unused, nodiscard]] te::Engine& getEngine() noexcept
    {
        return *engine;
    }

    [[maybe_unused, nodiscard]] const te::Edit& getEdit() const noexcept
    {
        return *edit;
    }

    [[maybe_unused, nodiscard]] te::Edit& getEdit() noexcept
    {
        return *edit;
    }

    [[maybe_unused, nodiscard]] const te::TransportControl&
    getTransport() const noexcept
    {
        return *transport;
    }

    [[maybe_unused, nodiscard]] te::TransportControl&
    getTransport() noexcept
    {
        return *transport;
    }

    [[maybe_unused, nodiscard]] const te::Project& getProject() const noexcept
    {
        return *project;
    }

    [[maybe_unused, nodiscard]] te::Project& getProject() noexcept
    {
        return *project;
    }


private:
    std::unique_ptr<te::Engine> engine;

    std::unique_ptr<te::Edit> edit;
    te::TransportControl*     transport;

    te::Project::Ptr project;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Context)
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_CONTEXT_CLASS_HPP
