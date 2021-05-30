#ifndef BLOOPER_EXT_VALUE_TREE_HPP
#define BLOOPER_EXT_VALUE_TREE_HPP

BLOOPER_EXT_NAMESPACE_BEGIN

// NOLINTNEXTLINE(misc-no-recursion)
[[maybe_unused]] inline void ensureAllItemsHaveIDs(
    const juce::ValueTree& root)
{
  if (root[te::IDs::uid].toString().isEmpty())
    juce::ValueTree(root).setProperty(
        te::IDs::uid,
        juce::String::toHexString(
            juce::Random().nextInt()),
        nullptr);

  for (int i = 0; i < root.getNumChildren(); ++i)
    ensureAllItemsHaveIDs(root.getChild(i));
}

BLOOPER_EXT_NAMESPACE_END

#endif // BLOOPER_EXT_VALUE_TREE_HPP
