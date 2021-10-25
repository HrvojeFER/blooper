#ifndef BLOOPER_FLAGGED_ASYNC_UPDATER_HPP
#define BLOOPER_FLAGGED_ASYNC_UPDATER_HPP
#pragma once

#include <blooper/internal/macros/namespaces.hpp>

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

  void markAndUpdate(juce::Atomic<bool>& flag)
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

  static bool compareAndReset(juce::Atomic<bool>& flag) noexcept
  {
    return flag.compareAndSetBool(false, true);
  }
};

BLOOPER_UTIL_NAMESPACE_END

#endif // BLOOPER_FLAGGED_ASYNC_UPDATER_HPP
