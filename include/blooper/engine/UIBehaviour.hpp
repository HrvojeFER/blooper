#ifndef BLOOPER_UI_BEHAVIOUR_HPP
#define BLOOPER_UI_BEHAVIOUR_HPP


#include <blooper/assets/assets.hpp>

#include <blooper/internal/internal.hpp>
#include <blooper/style/style.hpp>

#include <blooper/engine/devices/devices.hpp>
#include <blooper/engine/plugins/plugins.hpp>
#include <blooper/engine/settings/settings.hpp>
#include <blooper/engine/projects/projects.hpp>


BLOOPER_NAMESPACE_BEGIN

class UIBehaviour : public te::UIBehaviour
{
public:
    UIBehaviour() = default;

    std::unique_ptr<juce::Component> createPluginWindow(
            te::PluginWindowState& pws) override
    {
        if (auto ws = dynamic_cast<te::Plugin::WindowState*>(&pws))
            return PluginWindow::create(ws->plugin);

        return {};
    }

    void recreatePluginWindowContentAsync(
            te::Plugin& p) override
    {
        if (auto* w = dynamic_cast<PluginWindow*>(
                    p.windowState->pluginWindow.get()))
            return w->recreateEditorAsync();

        UIBehaviour::recreatePluginWindowContentAsync(p);
    }
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_UI_BEHAVIOUR_HPP
