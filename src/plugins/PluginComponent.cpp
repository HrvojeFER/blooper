#include <blooper/plugins/PluginComponent.hpp>

namespace blooper
{
PluginComponent::PluginComponent(EditViewState& evs, te::Plugin::Ptr p)
    : editViewState(evs), plugin(std::move(p))
{
    setButtonText(
            plugin->getName().substring(
                    0,
                    1));
}

void PluginComponent::clicked(const ModifierKeys& modifiers)
{
    editViewState.selectionManager.selectOnly(plugin.get());
    if (modifiers.isPopupMenu())
    {
        PopupMenu m;
        m.addItem("Delete",
                  [this] { plugin->deleteFromParent(); });
        m.showAt(this);
    }
    else
    {
        plugin->showWindowExplicitly();
    }
}
} // namespace blooper
