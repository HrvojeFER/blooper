#ifndef BLOOPER_EXT_VALUE_TREE_HPP
#define BLOOPER_EXT_VALUE_TREE_HPP

BLOOPER_EXT_NAMESPACE_BEGIN

// NOLINTNEXTLINE(misc-no-recursion)
[[maybe_unused]] inline void ensureAllItemsHaveIDs(
    const juce::ValueTree& folder)
{
  if (folder[te::IDs::uid].toString().isEmpty())
    juce::ValueTree(folder).setProperty(
        te::IDs::uid,
        juce::String::toHexString(
            juce::Random().nextInt()),
        nullptr);

  for (int i = 0; i < folder.getNumChildren(); ++i)
    ensureAllItemsHaveIDs(folder.getChild(i));
}

BLOOPER_EXT_NAMESPACE_END

#endif // BLOOPER_EXT_VALUE_TREE_HPP
