#ifndef BLOOPER_PLUGIN_TREE_HPP
#define BLOOPER_PLUGIN_TREE_HPP

#include <blooper/internal/abstract/contextual.hpp>
#include <blooper/internal/abstract/visitors.hpp>

BLOOPER_NAMESPACE_BEGIN

class PluginTreeBase : public ContextualBase
{
 public:
  explicit PluginTreeBase(AbstractContext& context);

  ~PluginTreeBase() override;


  [[nodiscard]] virtual juce::String getName() const = 0;

  [[nodiscard]] inline int getId() const;


  template<VisitDepth Depth = defaultVisitDepth, typename TVisitor>
  [[maybe_unused]] void visit(TVisitor visitor);
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


  template<VisitDepth Depth = defaultVisitDepth, typename TVisitor>
  [[maybe_unused]] void visitGroup(TVisitor visitor);


 private:
  juce::String name;

  juce::OwnedArray<PluginTreeBase> items;


  void populateFrom(juce::KnownPluginList::PluginTree& list);

  void addOwned(PluginTreeBase* item);


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

BLOOPER_NAMESPACE_END

BLOOPER_DETAIL_NAMESPACE_BEGIN

template<VisitDepth Depth, typename TVisitor>
[[maybe_unused]] inline void visit(
    PluginTreeBase* root,
    TVisitor        visitor);

template<VisitDepth Depth, typename TVisitor>
[[maybe_unused]] inline bool visit(
    PluginTreeGroup* root,
    TVisitor         visitor);

BLOOPER_DETAIL_NAMESPACE_END

BLOOPER_NAMESPACE_BEGIN

[[nodiscard]] inline int PluginTreeBase::getId() const
{
  return this->getName().hashCode();
}

template<VisitDepth Depth, typename TVisitor>
[[maybe_unused]] inline void PluginTreeBase::visit(TVisitor visitor)
{
  static_assert(
      meta::satisfies_any(
          isVisitorOf ^ meta::type_c<PluginTreeGroup&>,
          isVisitorOf ^ meta::type_c<PluginTreeItem&>)(
          BLOOPER_TYPEID(visitor)),
      "onGroup must be a Visitor of PluginTreeGroup& or PluginTreeItem&.");

  detail::visit<Depth>(this, move(visitor));
}


template<VisitDepth Depth, typename TVisitor>
[[maybe_unused]] inline void PluginTreeGroup::visitGroup(TVisitor visitor)
{
  static_assert(
      meta::satisfies_any(
          isVisitorOf ^ meta::type_c<PluginTreeGroup&>,
          isVisitorOf ^ meta::type_c<PluginTreeItem&>)(
          BLOOPER_TYPEID(visitor)),
      "onGroup must be a Visitor of PluginTreeGroup& or PluginTreeItem&.");

  detail::visit<Depth>(this, move(visitor));
}

BLOOPER_NAMESPACE_END

BLOOPER_DETAIL_NAMESPACE_BEGIN

template<VisitDepth Depth, typename TVisitor>
[[maybe_unused]] inline void visit(PluginTreeBase* root, TVisitor visitor)
{
  if (auto group = dynamic_cast<PluginTreeGroup*>(root))
    visit<Depth>(group, move(visitor));
}

#ifdef __JETBRAINS_IDE__
  #pragma clang diagnostic push
  #pragma ide diagnostic   ignored "misc-no-recursion"
#endif // __JETBRAINS_IDE__

template<VisitDepth Depth, typename TVisitor>
[[maybe_unused]] inline bool visit(
    PluginTreeGroup* root,
    TVisitor         visitor)
{
  for (auto subNode : *root)
  {
    if (auto subItem = dynamic_cast<class PluginTreeItem*>(subNode))
      if (!callIfVisitor(visitor, *subItem))
        return false;

    if (auto subGroup = dynamic_cast<class PluginTreeGroup*>(subNode))
    {
      if (!callIfVisitor(visitor, *subGroup))
        return false;

      if constexpr (Depth == VisitDepth::deep)
        if (!visit<Depth>(subGroup, visitor))
          return false;
    }
  }

  return true;
}

#ifdef __JETBRAINS_IDE__
  #pragma clang diagnostic pop
#endif // __JETBRAINS_IDE__

BLOOPER_DETAIL_NAMESPACE_END

#endif // BLOOPER_PLUGIN_TREE_HPP
