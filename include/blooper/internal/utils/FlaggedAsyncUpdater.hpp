#ifndef BLOOPER_FLAGGED_ASYNC_UPDATER_HPP
#define BLOOPER_FLAGGED_ASYNC_UPDATER_HPP

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/abstract.hpp>
#include <blooper/internal/ext/ext.hpp>

BLOOPER_UTIL_NAMESPACE_BEGIN

class [[maybe_unused]] FlaggedAsyncUpdater :
    public juce::AsyncUpdater
{
 public:
  void markAndUpdate(bool& flag)
  {
    flag = true;
    triggerAsyncUpdate();
  }

  void markAndUpdate(JuceFlag& flag)
  {
    flag = true;
    triggerAsyncUpdate();
  }

  static bool compareAndReset(bool& flag) noexcept
  {
    if (!flag) return false;

    flag = false;
    return true;
  }

  static bool compareAndReset(JuceFlag& flag) noexcept
  {
    return flag.compareAndSetBool(false, true);
  }
};

BLOOPER_UTIL_NAMESPACE_END

#endif // BLOOPER_FLAGGED_ASYNC_UPDATER_HPP
