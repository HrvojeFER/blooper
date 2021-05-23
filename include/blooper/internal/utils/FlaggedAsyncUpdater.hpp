#ifndef BLOOPER_FLAGGED_ASYNC_UPDATER_HPP
#define BLOOPER_FLAGGED_ASYNC_UPDATER_HPP

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/abstract.hpp>

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

  static bool compareAndReset(bool& flag) noexcept
  {
    if (!flag)
      return false;

    flag = false;
    return true;
  }
};

BLOOPER_UTIL_NAMESPACE_END

#endif // BLOOPER_FLAGGED_ASYNC_UPDATER_HPP
