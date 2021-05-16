#include <blooper/context/behaviour/UIBehaviour.hpp>

#include <blooper/context/core/core.hpp>

#include <blooper/context/devices/devices.hpp>
#include <blooper/context/settings/settings.hpp>
#include <blooper/context/plugins/plugins.hpp>
#include <blooper/context/projects/projects.hpp>


BLOOPER_NAMESPACE_BEGIN

UIBehaviour::UIBehaviour(CoreContext& context)
    : context(context)
{ }

UIBehaviour::~UIBehaviour() = default;


std::unique_ptr<juce::Component> UIBehaviour::createPluginWindow(
        te::PluginWindowState& windowState)
{
    if (auto concreteWindowState = dynamic_cast<te::Plugin::WindowState*>(
                &windowState))
        return PluginWindow::create(concreteWindowState->plugin);

    return {};
}

void UIBehaviour::recreatePluginWindowContentAsync(
        te::Plugin& plugin)
{
    if (auto* pluginWindow = dynamic_cast<PluginWindow*>(
                plugin.windowState->pluginWindow.get()))
        return pluginWindow->recreateEditorAsync();

    UIBehaviour::recreatePluginWindowContentAsync(plugin);
}


void UIBehaviour::showProjectScreen()
{
    auto projectWindow = new ProjectsMenuWindow(getEngine());
    projectWindow->enterModalState(
            true,
            nullptr,
            true);
}

void UIBehaviour::showSettingsScreen()
{
    auto settingsWindow = new SettingsMenuWindow(getEngine());
    settingsWindow->enterModalState(
            true,
            nullptr,
            true);
}

BLOOPER_NAMESPACE_END
