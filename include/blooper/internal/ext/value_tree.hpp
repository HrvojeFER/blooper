#ifndef BLOOPER_EXT_VALUE_TREE_HPP
#define BLOOPER_EXT_VALUE_TREE_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/traits.hpp>
#include <blooper/internal/abstract/predicates.hpp>
#include <blooper/internal/abstract/visitors.hpp>


BLOOPER_EXT_NAMESPACE_BEGIN

[[maybe_unused, nodiscard]] inline auto
isInvalid(const juce::ValueTree& tree) noexcept(noexcept(!tree.isValid()))
    -> decltype(!tree.isValid())
{
  return !tree.isValid();
}


template<VisitDepth Depth = defaultVisitDepth, typename TVisitor>
[[maybe_unused]] inline void
visit(
    const juce::ValueTree& root,
    TVisitor               visitor)
{
  static_assert(
      BLOOPER_TYPEID(visitor) ^
          isVisitorOf ^
          meta::type_c<juce::ValueTree>,
      "juce::ValueTree visitor requires a Visitor of juce::ValueTree&.");


  // TODO: better

  constexpr auto _visit =
      meta::fix([](auto                   visit,
                   const juce::ValueTree& visited,
                   auto                   visitor) {
        for (auto node : visited)
        {
          if (isInvalid(node)) continue;

          if (callVisitor(visitor, node) == stopVisit)
            return stopVisit;

          if constexpr (Depth == VisitDepth::deep)
            if (visit(node, visitor) == stopVisit)
              return stopVisit;
        }

        return continueVisit;
      });

  if (isInvalid(root)) return;

  _visit(root, move(visitor));
}

template<typename TVisitor>
[[maybe_unused]] inline void
visitAncestors(
    const juce::ValueTree& root,
    TVisitor               visitor)
{
  static_assert(
      BLOOPER_TYPEID(visitor) ^
          isVisitorOf ^
          meta::type_c<juce::ValueTree>,
      "juce::ValueTree visitor requires a Visitor of juce::ValueTree.");


  if (isInvalid(root)) return;

  if (callVisitor(visitor, root) == stopVisit) return;

  visitAncestors(root.getParent(), move(visitor));
}

template<VisitDepth Depth = defaultVisitDepth, typename TPredicate>
[[maybe_unused]] inline juce::ValueTree
find(
    const juce::ValueTree& root,
    TPredicate             predicate)
{
  static_assert(
      BLOOPER_TYPEID(predicate) ^
          isPredicateOf ^
          meta::type_c<juce::ValueTree>,
      "juce::ValueTree find requires a Predicate of juce::ValueTree.");


  juce::ValueTree result;

  visit<Depth>(
      root,
      [&result, predicate = move(predicate)](
          const juce::ValueTree& node) {
        if (predicate(node))
        {
          result = node;
          return stopVisit;
        }

        return continueVisit;
      });

  return result;
}

template<typename TPredicate>
[[maybe_unused]] inline juce::ValueTree
findAncestor(
    const juce::ValueTree& root,
    TPredicate             predicate)
{
  static_assert(
      BLOOPER_TYPEID(predicate) ^
          isPredicateOf ^
          meta::type_c<juce::ValueTree>,
      "juce::ValueTree findAncestor requires a Predicate of juce::ValueTree.");


  juce::ValueTree result;

  visitAncestors(
      root,
      [&result, predicate = move(predicate)](
          const juce::ValueTree& node) {
        if (predicate(node))
        {
          result = node;
          return stopVisit;
        }

        return continueVisit;
      });

  return result;
}

template<VisitDepth Depth = defaultVisitDepth,
         typename TPredicate,
         typename TProducer>
[[maybe_unused]] inline auto findOrCreate(
    juce::ValueTree& root,
    TPredicate       predicate,
    TProducer        producer)
{
  static_assert(
      BLOOPER_TYPEID(producer) ^
          meta::traits::is_producer_of ^
          meta::type_c<juce::ValueTree>,
      "juce::ValueTree findOrCreate requires a producer of juce::ValueTree");

  auto child = find<Depth>(root, move(predicate));

  if (isInvalid(child))
    child = producer();

  return move(child);
}


[[maybe_unused]] inline auto
ensureAllItemsHaveUids(const juce::ValueTree& root)
{
  visit(
      root,
      [](juce::ValueTree node) {
        if (node[te::IDs::uid].toString().isEmpty())
          node.setProperty(
              te::IDs::uid,
              juce::String::toHexString(
                  juce::Random().nextInt()),
              nullptr);
      });
}

[[maybe_unused]] inline auto
findUid(
    const juce::ValueTree& root,
    const juce::String&    id)
{
  return find(
      root,
      [&id](const juce::ValueTree& node) {
        return node[te::IDs::uid].toString() == id;
      });
}


[[maybe_unused]] inline std::unique_ptr<juce::File>
ensureExistingDirectory(const juce::File& file)
{
  if (!file.exists()) file.createDirectory();

  return std::make_unique<juce::File>(file);
}

[[maybe_unused]] inline std::unique_ptr<juce::File>
ensureExistingFile(const juce::File& file)
{
  if (!file.existsAsFile()) file.create();

  return std::make_unique<juce::File>(file);
}

[[maybe_unused]] inline std::unique_ptr<juce::PropertiesFile>
ensureValidStateFile(const juce::File& file)
{
  juce::PropertiesFile::Options options;
  options.millisecondsBeforeSaving = 2000;
  options.storageFormat = juce::PropertiesFile::storeAsXML;
  options.commonToAllUsers = false;

  // If not valid, just delete it and it will create a new one when needed.
  if (!juce::PropertiesFile(file, options).isValidFile())
    file.deleteFile();

  return std::make_unique<juce::PropertiesFile>(file, options);
}

[[maybe_unused]] inline juce::ValueTree
ensureValidState(
    juce::PropertiesFile&   file,
    const juce::String&     key,
    const juce::Identifier& id)
{
  juce::ValueTree state{};

  auto xml = file.getXmlValue(key);

  if (!xml)
  {
    state = State(id);
    xml = state.createXml();

    file.setValue(key, xml.get());
  }
  else
  {
    state = juce::ValueTree::fromXml(*xml);
  }


  if (!state.isValid() || !state.hasType(id))
  {
    state = State(id);
    file.setValue(key, state.createXml().get());
  }

  return state;
}

template<typename T, typename TDef = T>
[[maybe_unused]] inline auto
referTo(
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
