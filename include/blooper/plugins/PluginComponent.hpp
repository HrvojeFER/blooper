#ifndef BLOOPER_PLUGIN_COMPONENT_HPP
#define BLOOPER_PLUGIN_COMPONENT_HPP


#include <blooper/internal/internal.hpp>


BLOOPER_NAMESPACE_BEGIN

class PluginComponent : public juce::TextButton
{
public:
    PluginComponent(EditViewState&, te::Plugin::Ptr);
    ~PluginComponent() override = default;

    void clicked(const juce::ModifierKeys& modifiers) override;


private:
    EditViewState&  editViewState;
    te::Plugin::Ptr plugin;
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_PLUGIN_COMPONENT_HPP
