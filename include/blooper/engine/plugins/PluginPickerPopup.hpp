#ifndef BLOOPER_PLUGIN_PICKER_POPUP_HPP
#define BLOOPER_PLUGIN_PICKER_POPUP_HPP


BLOOPER_NAMESPACE_BEGIN

class PluginPickerPopup : public juce::PopupMenu
{
public:
    PluginPickerPopup() = default;

    explicit PluginPickerPopup(PluginTreeGroup& node)
    {
        for (int i = 0; i < node.getNumSubItems(); ++i)
            if (auto subNode =
                        dynamic_cast<PluginTreeGroup*>(node.getSubItem(i)))
                addSubMenu(subNode->name,
                           PluginPickerPopup(*subNode),
                           true);

        for (int i = 0; i < node.getNumSubItems(); ++i)
            if (auto subType =
                        dynamic_cast<PluginTreeItem*>(node.getSubItem(i)))
                addItem(subType->getUniqueName().hashCode(),
                        subType->desc.name,
                        true,
                        false);
    }

    static PluginTreeItem* findType(PluginTreeGroup& node, int hash)
    {
        for (int i = 0; i < node.getNumSubItems(); ++i)
            if (auto subNode =
                        dynamic_cast<PluginTreeGroup*>(node.getSubItem(i)))
                if (auto* t = findType(*subNode, hash))
                    return t;

        for (int i = 0; i < node.getNumSubItems(); ++i)
            if (auto t =
                        dynamic_cast<PluginTreeItem*>(node.getSubItem(i)))
                if (t->getUniqueName().hashCode() == hash)
                    return t;

        return nullptr;
    }

    PluginTreeItem* runMenu(PluginTreeGroup& node)
    {
        int res = show();

        if (res == 0)
            return nullptr;

        return findType(node, res);
    }
};


static inline te::Plugin::Ptr showMenuAndCreatePlugin(te::Edit& edit)
{
    if (auto tree = ext::engine::createPluginTree(edit.engine))
    {
        PluginTreeGroup root(
                edit,
                *tree,
                te::Plugin::Type::allPlugins);
        PluginPickerPopup m(root);

        if (auto type = m.runMenu(root))
            return type->create(edit);
    }

    return {};
}

BLOOPER_NAMESPACE_END


#endif // BLOOPER_PLUGIN_PICKER_POPUP_HPP
