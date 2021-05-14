#ifndef BLOOPER_PROPERTY_STORAGE_HPP
#define BLOOPER_PROPERTY_STORAGE_HPP


#include <blooper/assets/assets.hpp>

#include <blooper/internal/internal.hpp>
#include <blooper/style/style.hpp>

#include <blooper/context/devices/devices.hpp>
#include <blooper/context/plugins/plugins.hpp>
#include <blooper/context/settings/settings.hpp>
#include <blooper/context/projects/projects.hpp>


BLOOPER_NAMESPACE_BEGIN

class PropertyStorage : public te::PropertyStorage
{
public:
    ~PropertyStorage() override;


private:
    PropertyStorage();


    [[maybe_unused, nodiscard]] te::Engine& getEngine() noexcept
    {
        return *engine;
    }

    [[maybe_unused, nodiscard]] const te::Engine& getEngine() const noexcept
    {
        return *engine;
    }


    using EngineRef = te::Engine*;
    EngineRef engine;


    friend class Context;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PropertyStorage)
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_PROPERTY_STORAGE_HPP
