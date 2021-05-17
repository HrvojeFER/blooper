#ifndef BLOOPER_PLUGIN_TREE_HPP
#define BLOOPER_PLUGIN_TREE_HPP


BLOOPER_NAMESPACE_BEGIN

static inline constexpr auto internalPluginFormatName = "BlooperInternal";
static inline constexpr auto internalPluginUniqueIdSuffix = "_blooper_internal";


// TODO: cleanup

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

BLOOPER_NAMESPACE_END


#endif // BLOOPER_PLUGIN_TREE_HPP
