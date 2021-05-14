#ifndef BLOOPER_PLUGIN_COMPONENT_HPP
#define BLOOPER_PLUGIN_COMPONENT_HPP


#include <blooper/internal/internal.hpp>


BLOOPER_NAMESPACE_BEGIN

class PluginComponent : public juce::TextButton
{
public:
    using PluginRef = te::Plugin::Ptr;


    PluginComponent(PluginRef);

    ~PluginComponent() override = default;


    [[nodiscard]] const PluginRef& getPlugin() const
    {
        return plugin;
    }


    void clicked(const juce::ModifierKeys& modifiers) override;


private:
    PluginRef plugin;
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_PLUGIN_COMPONENT_HPP
