#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

UIBehaviour::UIBehaviour(AbstractCoreContext& context)
    : CoreContextualBase(context)
{
}


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
  auto projectWindow = new ProjectsMenuWindow(getContext());

  projectWindow->enterModalState(
      true,
      nullptr,
      true);
}

void UIBehaviour::showSettingsScreen()
{
  auto settingsWindow = new SettingsMenuWindow(getContext());

  settingsWindow->enterModalState(
      true,
      nullptr,
      true);
}

BLOOPER_NAMESPACE_END
