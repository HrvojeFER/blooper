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
  {
    PluginEditorWindow::Options windowOptions{};

    return showPluginEditorWindow(
        getContext(),
        concreteWindowState->plugin,
        std::move(windowOptions));
  }

  return te::UIBehaviour::createPluginWindow(windowState);
}

void UIBehaviour::recreatePluginWindowContentAsync(
    te::Plugin& plugin)
{
  if (auto window = dynamic_cast<PluginEditorWindow*>(
          plugin.windowState->pluginWindow.get()))
    return window->recreateContentAsync();

  return te::UIBehaviour::recreatePluginWindowContentAsync(plugin);
}


void UIBehaviour::showProjectScreen()
{
  showProjectsMenu(getContext());
}

void UIBehaviour::showSettingsScreen()
{
  showSettingsMenu(getContext());
}

BLOOPER_NAMESPACE_END
