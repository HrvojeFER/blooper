#ifndef BLOOPER_EXT_VALUE_TREE_HPP
#define BLOOPER_EXT_VALUE_TREE_HPP

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/abstract.hpp>

BLOOPER_EXT_NAMESPACE_BEGIN

template<typename TOnNode>
[[maybe_unused]] inline void visit(
    const JuceValueTree& root,
    TOnNode              onNode)
{
  static_assert(
      isInvokeable(
          meta::typeid_(onNode),
          meta::type_c<juce::ValueTree>),
      "juce::ValueTree visitor requires an Invokable with juce::ValueTree.");


  onNode(root);

  for (auto node : root)
    visit(node, onNode);
}

template<typename TOnNode>
[[maybe_unused]] inline void visit_ancestors(
    const JuceValueTree& root,
    TOnNode              onNode)
{
  static_assert(
      isInvokeable(
          meta::typeid_(onNode),
          meta::type_c<juce::ValueTree>),
      "juce::ValueTree visitor requires an Invokable with juce::ValueTree.");


  onNode(root);

  if (auto parent = root.getParent(); parent.isValid())
    visit_ancestors(parent, onNode);
}

template<typename TPredicate>
[[maybe_unused]] inline juce::ValueTree find(
    const JuceValueTree& root,
    TPredicate           predicate)
{
  static_assert(
      meta::traits::is_convertible(
          meta::traits::return_type(
              meta::typeid_(predicate),
              meta::type_c<juce::ValueTree>),
          meta::type_c<bool>),
      "juce::ValueTree visitor requires an Invokable with juce::ValueTree.");


  if (predicate(root)) return root;

  for (auto node : root)
    if (auto result = find(node, predicate); result.isValid())
      return result;

  return {};
}

template<typename TPredicate>
[[maybe_unused]] inline juce::ValueTree find_ancestor(
    const JuceValueTree& root,
    TPredicate           predicate)
{
  static_assert(
      meta::traits::is_convertible(
          meta::traits::return_type(
              meta::typeid_(predicate),
              meta::type_c<juce::ValueTree>),
          meta::type_c<bool>),
      "juce::ValueTree visitor requires an Invokable with juce::ValueTree.");


  if (predicate(root)) return root;

  if (auto parent = root.getParent(); parent.isValid())
    if (auto result = find(parent, predicate); result.isValid())
      return result;

  return {};
}


[[maybe_unused]] inline auto ensureAllItemsHaveUIDs(
    const JuceValueTree& root)
{
  visit(
      root,
      [](JuceValueTree node) {
        if (node[te::IDs::uid].toString().isEmpty())
          node.setProperty(
              te::IDs::uid,
              JuceString::toHexString(
                  juce::Random().nextInt()),
              nullptr);
      });
}

[[maybe_unused]] inline auto findUID(
    const JuceValueTree& root,
    const JuceString&    id)
{
  return find(
      root,
      [&id](const JuceValueTree& node) {
        return node[te::IDs::uid].toString() == id;
      });
}

BLOOPER_EXT_NAMESPACE_END

#endif // BLOOPER_EXT_VALUE_TREE_HPP
