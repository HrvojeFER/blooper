#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

PluginTreeBase::PluginTreeBase(AbstractContext& context)
    : ContextualBase(context)
{
}

PluginTreeBase::~PluginTreeBase() = default;


PluginTreeGroup::PluginTreeGroup(AbstractContext& context)
    : PluginTreeBase(context),
      name("Plugins")
{
  addOwned(createBuiltinsGroup(context));
  addOwned(createRacksGroup(context));

  auto tree =
      juce::KnownPluginList::createTree(
          getContext()
              .getEngine()
              .getPluginManager()
              .knownPluginList
              .getTypes(),
          juce::KnownPluginList::sortByManufacturer);

  populateFrom(*tree);
}

// NOLINTNEXTLINE(misc-no-recursion)
void PluginTreeGroup::populateFrom(juce::KnownPluginList::PluginTree& tree)
{
  for (auto subTree : tree.subFolders)
  {
    if (subTree->plugins.size() > 0 || subTree->subFolders.size() > 0)
    {
      auto subGroup =
          new PluginTreeGroup(
              getContext(),
              subTree->folder);
      subGroup->populateFrom(*subTree);

      addOwned(subGroup);
    }
  }

  for (const auto& description : tree.plugins)
    addOwned(
        new PluginTreeItem(
            getContext(),
            description));
}

PluginTreeGroup::PluginTreeGroup(AbstractContext& context, juce::String name)
    : PluginTreeBase(context),
      name(std::move(name))
{
  jassert(name.isNotEmpty());
}

size_t PluginTreeGroup::size() const noexcept
{
  return items.size();
}

PluginTreeBase* PluginTreeGroup::operator[](size_t i) const noexcept
{
  return items[static_cast<int>(i)];
}

PluginTreeBase** PluginTreeGroup::begin() noexcept
{
  return items.begin();
}

PluginTreeBase** PluginTreeGroup::end() noexcept
{
  return items.end();
}

void PluginTreeGroup::addOwned(PluginTreeBase* item)
{
  items.add(item);
}

juce::String PluginTreeGroup::getName() const
{
  return name;
}

PluginTreeGroup* PluginTreeGroup::createBuiltinsGroup(AbstractContext& context)
{
  constexpr auto add =
      [](bool areSynths = false, bool arePlugins = false) {
        return [areSynths, arePlugins](
                   AbstractContext& context,
                   PluginTreeGroup& tree,
                   const auto&      plugins) {
          auto suffix =
              juce::String(areSynths ?
                               synthPluginUniqueIdSuffix :
                               effectPluginUniqueIdSuffix) +
              builtinUniqueIdSuffix +
              (arePlugins ?
                   pluginUniqueIdSuffix :
                   "");

          env::meta::for_each(
              plugins,
              [&context,
               suffix = std::move(suffix),
               areSynths,
               arePlugins,
               &tree,
               i = 0](auto plugin) mutable {
                tree.addOwned(
                    new PluginTreeItem(
                        context,

                        juce::String(++i) + suffix,

                        TRANS(decltype(plugin)::type::
                                  getPluginName()),

                        decltype(plugin)::type::
                            xmlTypeName,

                        areSynths,
                        arePlugins));
              });
        };
      };

  constexpr auto addBuiltinEffects = add(false, false);
  constexpr auto addBuiltinSynths = add(true, false);


  auto builtinsFolder =
      new PluginTreeGroup(
          context,
          builtinsFolderName);

  addBuiltinEffects(
      context,
      *builtinsFolder,
      builtinEffects);

  addBuiltinSynths(
      context,
      *builtinsFolder,
      builtinSynths);

  return builtinsFolder;
}

PluginTreeGroup* PluginTreeGroup::createRacksGroup(AbstractContext& context)
{
  auto racksFolder =
      new PluginTreeGroup(
          context,
          racksFolderName);

  racksFolder->addOwned(
      new PluginTreeItem(
          context,

          juce::String("0") + rackUniqueIdSuffix,
          TRANS("Create New Empty Rack"),

          te::RackInstance::xmlTypeName,

          false,
          false));

  const auto& racks = context.getEdit().getRackList().getTypes();
  std::for_each(
      racks.begin(),
      racks.end(),
      [&context, racksFolder, i = 0](auto rack) mutable {
        racksFolder->addOwned(
            new PluginTreeItem(
                context,

                juce::String(++i) + rackUniqueIdSuffix,

                rack->rackName,

                te::RackInstance::xmlTypeName,

                false,
                false));
      });

  return racksFolder;
}


PluginTreeItem::PluginTreeItem(
    AbstractContext&               context,
    const juce::PluginDescription& description)

    : PluginTreeBase(context),

      description(description),

      xmlType(te::ExternalPlugin::xmlTypeName),
      isPlugin(true)
{
  jassert(xmlType.isNotEmpty());
}

PluginTreeItem::PluginTreeItem(
    AbstractContext& context,

    juce::String uniqueId,
    juce::String name,

    juce::String xmlType,

    bool isSynth,
    bool isPlugin)

    : PluginTreeBase(context),

      description(),

      xmlType(std::move(xmlType)),
      isPlugin(isPlugin)
{
  jassert(xmlType.isNotEmpty());

  description.pluginFormatName =
      (uniqueId.endsWith(builtinUniqueIdSuffix) ||
       xmlType == te::RackInstance::xmlTypeName) ?
          builtinPluginFormatName :
          juce::String();

  description.name = std::move(name);
  description.category = std::move(xmlType);

  description.fileOrIdentifier = std::move(uniqueId);

  description.isInstrument = isSynth;
}

[[nodiscard]] juce::String PluginTreeItem::getName() const
{
  if (description.fileOrIdentifier.startsWith(
          te::RackType::getRackPresetPrefix()))
    return description.fileOrIdentifier;

  return description.createIdentifierString();
}

te::Plugin::Ptr PluginTreeItem::createPlugin()
{
  return getContext()
      .getEdit()
      .getPluginCache()
      .createNewPlugin(xmlType, description);
}

BLOOPER_NAMESPACE_END
