#ifndef BLOOPER_EXT_VALUE_TREE_HPP
#define BLOOPER_EXT_VALUE_TREE_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/traits.hpp>

BLOOPER_EXT_NAMESPACE_BEGIN

template<typename TOnNode>
[[maybe_unused]] inline void visit(
    const JuceValueTree& root,
    TOnNode              onNode)
{
  static_assert(
      isInvokable(
          meta::typeid_(onNode),
          meta::type_c<juce::ValueTree>),
      "juce::ValueTree visitor requires an Invokable with juce::ValueTree.");


  onNode(root);

  for (auto node : root)
    visit(node, onNode);
}

template<typename TOnNode>
[[maybe_unused]] inline void visitAncestors(
    const JuceValueTree& root,
    TOnNode              onNode)
{
  static_assert(
      isInvokable(
          meta::typeid_(onNode),
          meta::type_c<juce::ValueTree>),
      "juce::ValueTree visitor requires an Invokable with juce::ValueTree.");


  onNode(root);

  if (auto parent = root.getParent(); parent.isValid())
    visitAncestors(parent, onNode);
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
[[maybe_unused]] inline juce::ValueTree findAncestor(
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


[[maybe_unused]] inline auto ensureAllItemsHaveUids(
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

[[maybe_unused]] inline auto findUid(
    const JuceValueTree& root,
    const JuceString&    id)
{
  return find(
      root,
      [&id](const JuceValueTree& node) {
        return node[te::IDs::uid].toString() == id;
      });
}


[[maybe_unused]] inline std::unique_ptr<JuceFile> ensureExistingDirectory(
    const JuceFile& file)
{
  if (!file.exists()) file.createDirectory();

  return std::make_unique<JuceFile>(file);
}

[[maybe_unused]] inline std::unique_ptr<JuceFile> ensureExistingFile(
    const JuceFile& file)
{
  if (!file.existsAsFile()) file.create();

  return std::make_unique<JuceFile>(file);
}

[[maybe_unused]] inline std::unique_ptr<JuceXmlFile> ensureValidStateFile(
    const JuceFile& file)
{
  JuceXmlFile::Options options;
  options.millisecondsBeforeSaving = 2000;
  options.storageFormat = JuceXmlFile::storeAsXML;
  options.commonToAllUsers = false;

  // If not valid, just delete it and it will create a new one when needed.
  if (!JuceXmlFile(file, options).isValidFile())
    file.deleteFile();

  return std::make_unique<JuceXmlFile>(file, options);
}

[[maybe_unused]] inline JuceState ensureValidState(
    JuceXmlFile&               file,
    const JuceString&          key,
    const JuceStateIdentifier& id)
{
  JuceState state{};

  auto xml = file.getXmlValue(key);

  if (!xml)
  {
    state = State(id);
    xml = state.createXml();

    file.setValue(key, xml.get());
  }
  else
  {
    state = JuceState::fromXml(*xml);
  }


  if (!state.isValid() || !state.hasType(id))
  {
    state = State(id);
    file.setValue(key, state.createXml().get());
  }

  return state;
}

template<typename T, typename TDef = T>
[[maybe_unused]] inline auto referTo(
    juce::CachedValue<T>&  value,
    juce::ValueTree&       tree,
    const juce::Identifier id,
    juce::UndoManager*     undo = nullptr,
    const TDef&            def = {})
    -> decltype(auto)
{
  value.referTo(tree, id, undo, def);

  if (value.get() == def)
  {
    value.setValue(def, nullptr);
  }
}

BLOOPER_EXT_NAMESPACE_END

#endif // BLOOPER_EXT_VALUE_TREE_HPP
