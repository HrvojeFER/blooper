#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

UIBehaviour::UIBehaviour(AbstractCoreContext& context)
    : CoreContextualBase(context)
{
}


std::unique_ptr<JuceComponent> UIBehaviour::createPluginWindow(
    JucePluginWindowState& windowState)
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
    JucePlugin& plugin)
{
  if (auto window = dynamic_cast<PluginEditorWindow*>(
          plugin.windowState->pluginWindow.get()))
    return util::callAsync(
        [safeWindow =
             juce::Component::SafePointer<PluginEditorWindow>(
                 window)]() mutable {
          if (safeWindow) safeWindow->recreateContent();
        });

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
