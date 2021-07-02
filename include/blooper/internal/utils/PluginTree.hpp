#ifndef BLOOPER_PLUGIN_TREE_HPP
#define BLOOPER_PLUGIN_TREE_HPP

#include <blooper/internal/abstract/contextual.hpp>

BLOOPER_NAMESPACE_BEGIN

class PluginTreeBase : public ContextualBase
{
 public:
  explicit PluginTreeBase(AbstractContext& context);

  ~PluginTreeBase() override;


  [[nodiscard]] virtual juce::String getName() const = 0;

  [[nodiscard]] inline int getId() const;


  template<VisitDepth Depth = defaultVisitDepth,
           typename TOnGroup,
           typename TOnItem>
  [[maybe_unused]] void visit(
      TOnGroup onGroup,
      TOnItem  onItem);
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


#ifdef __JETBRAINS_IDE__
  #pragma clang diagnostic push
  #pragma ide diagnostic   ignored "HidingNonVirtualFunction"
#endif // __JETBRAINS_IDE__

  template<VisitDepth Depth = defaultVisitDepth,
           typename TOnGroup,
           typename TOnItem>
  [[maybe_unused]] void visit(
      TOnGroup onGroup,
      TOnItem  onItem);

#ifdef __JETBRAINS_IDE__
  #pragma clang diagnostic pop
#endif // __JETBRAINS_IDE__


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


[[nodiscard]] inline int PluginTreeBase::getId() const
{
  return this->getName().hashCode();
}

template<VisitDepth Depth,
         typename TOnGroup,
         typename TOnItem>
[[maybe_unused]] inline void PluginTreeBase::visit(
    TOnGroup onGroup,
    TOnItem  onItem)
{
  static_assert(
      meta::typeid_(onGroup) ^
          isVisitorOf ^
          meta::type_c<PluginTreeGroup&>,
      "onGroup must be a Visitor of PluginTreeGroup&.");

  static_assert(
      meta::typeid_(onItem) ^
          isVisitorOf ^
          meta::type_c<PluginTreeItem&>,
      "onItem must be a Visitor of PluginTreeItem&.");

  detail::visit<Depth>(
      this,
      move(onGroup),
      move(onItem));
}


#ifdef __JETBRAINS_IDE__
  #pragma clang diagnostic push
  #pragma ide diagnostic   ignored "HidingNonVirtualFunction"
#endif // __JETBRAINS_IDE__

template<VisitDepth Depth,
         typename TOnGroup,
         typename TOnItem>
[[maybe_unused]] inline void PluginTreeGroup::visit(
    TOnGroup onGroup,
    TOnItem  onItem)
{
  static_assert(
      meta::typeid_(onGroup) ^
          isVisitorOf ^
          meta::type_c<PluginTreeGroup&>,
      "onGroup must be a Visitor of PluginTreeGroup&.");

  static_assert(
      meta::typeid_(onItem) ^
          isVisitorOf ^
          meta::type_c<PluginTreeItem&>,
      "onItem must be a Visitor of PluginTreeItem&.");

  detail::visit<Depth>(
      this,
      move(onGroup),
      move(onItem));
}

#ifdef __JETBRAINS_IDE__
  #pragma clang diagnostic pop
#endif // __JETBRAINS_IDE__

BLOOPER_NAMESPACE_END

BLOOPER_DETAIL_NAMESPACE_BEGIN

template<VisitDepth Depth,
         typename TOnGroup,
         typename TOnItem>
[[maybe_unused]] inline void visit(
    PluginTreeBase* root,
    TOnGroup        onGroup,
    TOnItem         onItem)
{
  if (auto group = dynamic_cast<PluginTreeGroup*>(root))
  {
    bool shouldStop = false;
    visit<Depth>(
        group,
        move(onGroup),
        move(onItem),
        shouldStop);
  }
}

#ifdef __JETBRAINS_IDE__
  #pragma clang diagnostic push
  #pragma ide diagnostic   ignored "misc-no-recursion"
#endif // __JETBRAINS_IDE__

template<VisitDepth Depth,
         typename TOnGroup,
         typename TOnItem>
[[maybe_unused]] inline void visit(
    PluginTreeGroup* root,
    TOnGroup         onGroup,
    TOnItem          onItem,
    bool&            shouldStop)
{
  for (auto subNode : *root)
  {
    if (auto subItem = dynamic_cast<class PluginTreeItem*>(subNode))
    {
      if (!callVisitor(onItem, *subItem))
      {
        shouldStop = true;
        return;
      }
    }

    if (auto subGroup = dynamic_cast<class PluginTreeGroup*>(subNode))
    {
      if (!callVisitor(onGroup, *subGroup))
      {
        shouldStop = true;
        return;
      }

      if constexpr (Depth == VisitDepth::deep)
      {
        visit<Depth>(
            subGroup,
            onGroup,
            onItem,
            shouldStop);

        if (shouldStop) return;
      }
    }
  }
}

#ifdef __JETBRAINS_IDE__
  #pragma clang diagnostic pop
#endif // __JETBRAINS_IDE__

BLOOPER_DETAIL_NAMESPACE_END

#endif // BLOOPER_PLUGIN_TREE_HPP
