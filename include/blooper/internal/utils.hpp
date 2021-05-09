#ifndef BLOOPER_UTILS_HPP
#define BLOOPER_UTILS_HPP


BLOOPER_NAMESPACE_BEGIN

namespace utils
{
static inline void addAndMakeVisible(
        juce::Component&                     parent,
        const juce::Array<juce::Component*>& children)
{
    for (auto c : children)
        parent.addAndMakeVisible(c);
}

static inline juce::File findRecentEdit(const juce::File& dir)
{
    auto files = dir.findChildFiles(
            juce::File::findFiles,
            false,
            "*.tracktionedit");

    if (files.size() > 0)
    {
        files.sort();
        return files.getLast();
    }

    return {};
}

namespace
{
class FlaggedAsyncUpdater : public juce::AsyncUpdater
{
public:
    void markAndUpdate(bool& flag)
    {
        flag = true;
        triggerAsyncUpdate();
    }

    static bool compareAndReset(bool& flag) noexcept
    {
        if (!flag)
            return false;

        flag = false;
        return true;
    }
};
} // namespace
} // namespace Helpers

BLOOPER_NAMESPACE_END


#endif // BLOOPER_UTILS_HPP
