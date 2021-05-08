#ifndef BLOOPER_PLUGIN_COMPONENT_HPP
#define BLOOPER_PLUGIN_COMPONENT_HPP


#include <blooper/internal/internal.hpp>


namespace blooper
{
class PluginComponent : public TextButton
{
public:
    PluginComponent(EditViewState&, te::Plugin::Ptr);
    ~PluginComponent() override = default;

    void clicked(const ModifierKeys& modifiers) override;


private:
    EditViewState&  editViewState;
    te::Plugin::Ptr plugin;
};
} // namespace blooper


#endif // BLOOPER_PLUGIN_COMPONENT_HPP
