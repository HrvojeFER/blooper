#include <blooper/plugins/PluginComponent.hpp>


BLOOPER_NAMESPACE_BEGIN

PluginComponent::PluginComponent(EditViewState& evs, te::Plugin::Ptr p)
    : editViewState(evs), plugin(std::move(p))
{
    setButtonText(
            plugin->getName().substring(
                    0,
                    1));
}

void PluginComponent::clicked(const juce::ModifierKeys& modifiers)
{
    editViewState.selectionManager.selectOnly(plugin.get());
    if (modifiers.isPopupMenu())
    {
        juce::PopupMenu m;
        m.addItem("Delete",
                  [this] { plugin->deleteFromParent(); });
        m.showAt(this);
    }
    else
    {
        plugin->showWindowExplicitly();
    }
}

BLOOPER_NAMESPACE_END
