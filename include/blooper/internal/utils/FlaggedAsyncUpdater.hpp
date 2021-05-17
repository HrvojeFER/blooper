#ifndef BLOOPER_FLAGGED_ASYNC_UPDATER_HPP
#define BLOOPER_FLAGGED_ASYNC_UPDATER_HPP


BLOOPER_UTILS_NAMESPACE_BEGIN

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

BLOOPER_UTILS_NAMESPACE_END


#endif // BLOOPER_FLAGGED_ASYNC_UPDATER_HPP
