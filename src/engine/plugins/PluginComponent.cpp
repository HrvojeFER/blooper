#include <blooper/engine/plugins/PluginComponent.hpp>


BLOOPER_NAMESPACE_BEGIN

PluginComponent::PluginComponent(te::Plugin::Ptr p)
    : plugin(std::move(p))
{
    setButtonText(
            plugin->getName().substring(
                    0,
                    1));
}

void PluginComponent::clicked(const juce::ModifierKeys& modifiers)
{
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
