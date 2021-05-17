#ifndef BLOOPER_PLUGIN_COMPONENT_HPP
#define BLOOPER_PLUGIN_COMPONENT_HPP


#include <blooper/fwd.hpp>

#include <blooper/context/core/core.hpp>


BLOOPER_NAMESPACE_BEGIN

class PluginComponent : public CoreComponent
{
public:
    using PluginRef = te::Plugin::Ptr;


    PluginComponent(CoreContext& context, PluginRef plugin);


    [[nodiscard, maybe_unused]] inline const te::Plugin&
    getPlugin() const noexcept;


    void resized() override;


private:
    [[nodiscard, maybe_unused]] inline te::Plugin&
    getPlugin() noexcept;


    PluginRef plugin;


    class Button : public juce::TextButton
    {
    public:
        explicit Button(PluginComponent* parent);

        void clicked(const juce::ModifierKeys& modifiers) override;


    private:
        PluginComponent* parent;
    };

    Button button;
};


const te::Plugin& PluginComponent::getPlugin() const noexcept
{
    return *plugin;
}

te::Plugin& PluginComponent::getPlugin() noexcept
{
    return *plugin;
}

BLOOPER_NAMESPACE_END


#endif // BLOOPER_PLUGIN_COMPONENT_HPP
