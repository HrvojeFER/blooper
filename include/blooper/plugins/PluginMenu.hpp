#ifndef BLOOPER_PLUGIN_MENU_HPP
#define BLOOPER_PLUGIN_MENU_HPP


#include <blooper/internal/internal.hpp>


BLOOPER_NAMESPACE_BEGIN

static inline constexpr auto internalPluginFormatName = "BlooperInternal";
static inline constexpr auto internalPluginUniqueIdSuffix = "_blooper_internal";


class PluginTreeBase
{
public:
    virtual ~PluginTreeBase() = default;
    [[nodiscard]] virtual juce::String getUniqueName() const = 0;

    void addSubItem(PluginTreeBase* itm)
    {
        sub_items.add(itm);
    }
    int getNumSubItems()
    {
        return sub_items.size();
    }
    PluginTreeBase* getSubItem(int idx)
    {
        return sub_items[idx];
    }

private:
    juce::OwnedArray<PluginTreeBase> sub_items;
};


class PluginTreeItem : public PluginTreeBase
{
public:
    explicit PluginTreeItem(const juce::PluginDescription&);

    PluginTreeItem(const juce::String& uniqueId,
                   const juce::String& name,
                   juce::String        xmlType,
                   bool                isSynth,
                   bool                isPlugin);

    te::Plugin::Ptr create(te::Edit&) const;

    [[nodiscard]] juce::String getUniqueName() const override
    {
        if (desc.fileOrIdentifier.startsWith(
                    te::RackType::getRackPresetPrefix()))
            return desc.fileOrIdentifier;

        return desc.createIdentifierString();
    }

    juce::PluginDescription desc;
    juce::String            xmlType;
    bool                    isPlugin = true;

    JUCE_LEAK_DETECTOR(PluginTreeItem)
};


class PluginTreeGroup : public PluginTreeBase
{
public:
    explicit PluginTreeGroup(juce::String);

    PluginTreeGroup(
            te::Edit&,
            juce::KnownPluginList::PluginTree&,
            te::Plugin::Type);


    [[nodiscard]] juce::String getUniqueName() const override
    {
        return name;
    }

    juce::String name;


private:
    void populateFrom(juce::KnownPluginList::PluginTree&);
    void createBuiltInItems(int& num, te::Plugin::Type);


    JUCE_LEAK_DETECTOR(PluginTreeGroup)
};

class PluginMenu : public juce::PopupMenu
{
public:
    PluginMenu() = default;

    explicit PluginMenu(PluginTreeGroup& node)
    {
        for (int i = 0; i < node.getNumSubItems(); ++i)
            if (auto subNode =
                        dynamic_cast<PluginTreeGroup*>(node.getSubItem(i)))
                addSubMenu(subNode->name,
                           PluginMenu(*subNode),
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
    if (auto tree = EngineHelpers::createPluginTree(edit.engine))
    {
        PluginTreeGroup root(
                edit,
                *tree,
                te::Plugin::Type::allPlugins);
        PluginMenu m(root);

        if (auto type = m.runMenu(root))
            return type->create(edit);
    }

    return {};
}

BLOOPER_NAMESPACE_END


#endif // BLOOPER_PLUGIN_MENU_HPP
