#include <blooper/context/plugins/PluginTree.hpp>


BLOOPER_NAMESPACE_BEGIN

PluginTreeItem::PluginTreeItem(const juce::PluginDescription& d)
    : desc(d),
      xmlType(te::ExternalPlugin::xmlTypeName),
      isPlugin(true)
{
    jassert(xmlType.isNotEmpty());
}

PluginTreeItem::PluginTreeItem(
        const juce::String& uniqueId,
        const juce::String& name,
        juce::String        xmlType_,
        bool                isSynth,
        bool                isPlugin_)
    : xmlType(std::move(xmlType_)),
      isPlugin(isPlugin_)
{
    jassert(xmlType.isNotEmpty());
    desc.name = name;
    desc.fileOrIdentifier = uniqueId;
    desc.pluginFormatName =
            (uniqueId.endsWith(internalPluginUniqueIdSuffix) ||
             xmlType == te::RackInstance::xmlTypeName) ?
                    internalPluginFormatName :
                    juce::String();
    desc.category = xmlType;
    desc.isInstrument = isSynth;
}

te::Plugin::Ptr PluginTreeItem::create(te::Edit& ed) const
{
    return ed.getPluginCache().createNewPlugin(xmlType, desc);
}


PluginTreeGroup::PluginTreeGroup(
        te::Edit&                          edit,
        juce::KnownPluginList::PluginTree& tree,
        te::Plugin::Type                   types)
    : name("Plugins")
{
    {
        int num = 1;

        auto builtinFolder =
                new PluginTreeGroup(TRANS("Builtin Plugins"));
        addSubItem(builtinFolder);
        builtinFolder->createBuiltInItems(num, types);
    }

    {
        auto racksFolder =
                new PluginTreeGroup(TRANS("Plugin Racks"));
        addSubItem(racksFolder);

        racksFolder->addSubItem(
                new PluginTreeItem(
                        juce::String(
                                te::RackType::getRackPresetPrefix()) +
                                "-1",
                        TRANS("Create New Empty Rack"),
                        te::RackInstance::xmlTypeName,
                        false,
                        false));

        int i = 0;
        for (auto rf : edit.getRackList().getTypes())
            racksFolder->addSubItem(
                    new PluginTreeItem(
                            "RACK__" + juce::String(i++),
                            rf->rackName,
                            te::RackInstance::xmlTypeName,
                            false,
                            false));
    }

    populateFrom(tree);
}

PluginTreeGroup::PluginTreeGroup(juce::String s) : name(std::move(s))
{
    jassert(name.isNotEmpty());
}

void PluginTreeGroup::populateFrom(juce::KnownPluginList::PluginTree& tree)
{
    for (auto subTree : tree.subFolders)
    {
        if (subTree->plugins.size() > 0 || subTree->subFolders.size() > 0)
        {
            auto fs = new PluginTreeGroup(subTree->folder);
            addSubItem(fs);

            fs->populateFrom(*subTree);
        }
    }

    for (const auto& pd : tree.plugins)
        addSubItem(new PluginTreeItem(pd));
}


template<class FilterClass>
void addInternalPlugin(PluginTreeBase& item, int& num, bool synth = false)
{
    item.addSubItem(
            new PluginTreeItem(
                    juce::String(num++) + internalPluginUniqueIdSuffix,
                    TRANS(FilterClass::getPluginName()),
                    FilterClass::xmlTypeName,
                    synth,
                    false));
}

void PluginTreeGroup::createBuiltInItems(int& num, te::Plugin::Type types)
{
    addInternalPlugin<te::VolumeAndPanPlugin>(*this, num);
    addInternalPlugin<te::LevelMeterPlugin>(*this, num);
    addInternalPlugin<te::EqualiserPlugin>(*this, num);
    addInternalPlugin<te::ReverbPlugin>(*this, num);
    addInternalPlugin<te::DelayPlugin>(*this, num);
    addInternalPlugin<te::ChorusPlugin>(*this, num);
    addInternalPlugin<te::PhaserPlugin>(*this, num);
    addInternalPlugin<te::CompressorPlugin>(*this, num);
    addInternalPlugin<te::PitchShiftPlugin>(*this, num);
    addInternalPlugin<te::LowPassPlugin>(*this, num);
    addInternalPlugin<te::MidiModifierPlugin>(*this, num);
    addInternalPlugin<te::MidiPatchBayPlugin>(*this, num);
    addInternalPlugin<te::PatchBayPlugin>(*this, num);
    addInternalPlugin<te::AuxSendPlugin>(*this, num);
    addInternalPlugin<te::AuxReturnPlugin>(*this, num);
    addInternalPlugin<te::TextPlugin>(*this, num);
    addInternalPlugin<te::FreezePointPlugin>(*this, num);

#if TRACKTION_ENABLE_REWIRE
    addInternalPlugin<te::ReWirePlugin>(*this, num, true);
#endif

    if (types == te::Plugin::Type::allPlugins)
    {
        addInternalPlugin<te::SamplerPlugin>(*this, num, true);
        addInternalPlugin<te::FourOscPlugin>(*this, num, true);
    }

    addInternalPlugin<te::InsertPlugin>(*this, num);

#if ENABLE_INTERNAL_PLUGINS
    for (auto& d : PluginTypeBase::getAllPluginDescriptions())
        if (isPluginAuthorised(d))
            addSubItem(new PluginTreeItem(d));
#endif
}

BLOOPER_NAMESPACE_END
