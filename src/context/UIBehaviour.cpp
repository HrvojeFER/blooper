#include <blooper/context/UIBehaviour.hpp>


BLOOPER_NAMESPACE_BEGIN

UIBehaviour::UIBehaviour()
    : engine(nullptr)
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
    auto projectWindow = std::make_unique<ProjectsMenuWindow>(getEngine());
}

void UIBehaviour::showSettingsScreen()
{
    auto projectWindow = std::make_unique<SettingsMenuWindow>(getEngine());
}

BLOOPER_NAMESPACE_END
