#ifndef BLOOPER_PLUGIN_TREE_HPP
#define BLOOPER_PLUGIN_TREE_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

#ifdef __JETBRAINS_IDE__
  #pragma clang diagnostic push
  #pragma ide diagnostic   ignored "cert-err58-cpp"
#endif

static inline constexpr auto builtinPluginFormatName = "Internal";

static inline constexpr auto builtinUniqueIdSuffix = "_internal";
static inline constexpr auto effectPluginUniqueIdSuffix = "_effect";
static inline constexpr auto synthPluginUniqueIdSuffix = "_synth";
static inline constexpr auto pluginUniqueIdSuffix = "_plugin";
static inline constexpr auto rackUniqueIdSuffix = "_rack";

static inline const auto builtinsFolderName = TRANS("Builtins");
static inline const auto racksFolderName = TRANS("Racks");

static inline constexpr auto builtinEffects =
    env::meta::make_tuple(
        env::meta::type_c<te::VolumeAndPanPlugin>,
        env::meta::type_c<te::EqualiserPlugin>,
        env::meta::type_c<te::ReverbPlugin>,
        env::meta::type_c<te::DelayPlugin>,
        env::meta::type_c<te::ChorusPlugin>,
        env::meta::type_c<te::PhaserPlugin>,
        env::meta::type_c<te::CompressorPlugin>,
        env::meta::type_c<te::PitchShiftPlugin>,
        env::meta::type_c<te::LowPassPlugin>,
        env::meta::type_c<te::MidiModifierPlugin>,
        env::meta::type_c<te::MidiPatchBayPlugin>,
        env::meta::type_c<te::PatchBayPlugin>,
        env::meta::type_c<te::AuxSendPlugin>,
        env::meta::type_c<te::AuxReturnPlugin>,
        env::meta::type_c<te::TextPlugin>,
        env::meta::type_c<te::FreezePointPlugin>,
        env::meta::type_c<te::InsertPlugin>);

static inline constexpr auto builtinSynths =
    env::meta::make_tuple(
        env::meta::type_c<te::SamplerPlugin>,
        env::meta::type_c<te::FourOscPlugin>);

#ifdef __JETBRAINS_IDE__
  #pragma clang diagnostic pop
#endif


class PluginTreeBase : public ContextualBase
{
 public:
  explicit PluginTreeBase(AbstractContext& context);

  ~PluginTreeBase() override;


  [[nodiscard]] virtual juce::String getName() const = 0;
};


class PluginTreeGroup : public PluginTreeBase
{
 public:
  explicit PluginTreeGroup(AbstractContext& context);

  PluginTreeGroup(AbstractContext& context, juce::String name);


  [[nodiscard]] juce::String getName() const override;


  [[nodiscard]] size_t size() const noexcept;

  [[nodiscard]] PluginTreeBase* operator[](size_t i) const noexcept;


  [[nodiscard]] PluginTreeBase** begin() noexcept;

  [[nodiscard]] PluginTreeBase** end() noexcept;

  void addOwned(PluginTreeBase* item);


 private:
  juce::String name;

  juce::OwnedArray<PluginTreeBase> items;


  void populateFrom(juce::KnownPluginList::PluginTree& list);


  [[nodiscard]] static PluginTreeGroup* createBuiltinsGroup(
      AbstractContext& context);

  [[nodiscard]] static PluginTreeGroup* createRacksGroup(
      AbstractContext& context);


  JUCE_LEAK_DETECTOR(PluginTreeGroup)
};


class PluginTreeItem : public PluginTreeBase
{
 public:
  PluginTreeItem(
      AbstractContext&               context,
      const juce::PluginDescription& description);

  PluginTreeItem(
      AbstractContext& context,

      juce::String uniqueId,
      juce::String name,

      juce::String xmlType,

      bool isSynth,
      bool isPlugin);


  [[nodiscard]] juce::String getName() const override;


  [[nodiscard]] te::Plugin::Ptr createPlugin();


 private:
  juce::PluginDescription description;

  juce::String xmlType;
  bool         isPlugin = true;


  JUCE_LEAK_DETECTOR(PluginTreeItem)
};


enum class PluginTreeVisitType
{
  deep,
  shallow
};

template<typename TOnGroup, typename TOnItem>
[[maybe_unused]] static inline constexpr auto isPluginTreeVisitNoexcept =
    noexcept(std::declval<TOnGroup>()(
                 std::declval<class PluginTreeGroup&>()),
             std::declval<TOnItem>()(
                 std::declval<class PluginTreeItem&>()));

template<PluginTreeVisitType VisitType = PluginTreeVisitType::shallow,
         typename TOnGroup,
         typename TOnItem>
[[maybe_unused]] auto visit(
    PluginTreeBase* base,
    TOnGroup        onGroup,
    TOnItem         onItem) noexcept(isPluginTreeVisitNoexcept<TOnGroup, TOnItem>)
{
  static_assert(
      isInvokeable(
          env::meta::typeid_(onGroup),
          env::meta::type_c<PluginTreeGroup&>),
      "onSubGroup must be invokable with PluginTreeGroup&.");

  static_assert(
      isInvokeable(
          env::meta::typeid_(onItem),
          env::meta::type_c<PluginTreeItem&>),
      "onSubItem must be invokable with PluginTreeItem&.");


  if (auto group = dynamic_cast<PluginTreeGroup*>(base))
  {
    for (auto subNode : *group)
      if (auto subGroup = dynamic_cast<class PluginTreeGroup*>(subNode))
      {
        onGroup(*subGroup);

        if constexpr (VisitType == PluginTreeVisitType::deep)
        {
          visit(group, onGroup, onItem);
        }
      }

    for (auto subNode : *group)
      if (auto subItem = dynamic_cast<class PluginTreeItem*>(subNode))
        onItem(*subItem);
  }

  if (auto item = dynamic_cast<PluginTreeItem*>(base))
    onItem(*item);
}

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PLUGIN_TREE_HPP
