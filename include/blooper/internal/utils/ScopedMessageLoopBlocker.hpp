#ifndef BLOOPER_MESSAGE_LOOP_BLOCKER_HPP
#define BLOOPER_MESSAGE_LOOP_BLOCKER_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/contextual.hpp>

#include <blooper/internal/utils/ScopedDPIAwarenessDisabler.hpp>

BLOOPER_UTIL_NAMESPACE_BEGIN

inline bool needsMessageLoopBlocking(
    AbstractCoreContext& context,
    const te::Plugin&    plugin) noexcept
{
  return needsDPIAwarenessDisabling(context, plugin);
}


struct ScopedMessageLoopBlocker
{
 public:
  inline ScopedMessageLoopBlocker() noexcept = default;
  inline virtual ~ScopedMessageLoopBlocker() noexcept = default;

  [[maybe_unused, nodiscard]] inline virtual bool isReal() const noexcept = 0;
};

struct RealScopedMessageLoopBlocker final :
    public ScopedMessageLoopBlocker,
    private JuceComponent
{
 public:
  inline RealScopedMessageLoopBlocker()
      : ScopedMessageLoopBlocker()
  {
    this->enterModalState(false);
  }

  inline ~RealScopedMessageLoopBlocker() noexcept final
  {
    this->exitModalState(0);
  }


  [[maybe_unused, nodiscard]] inline bool isReal() const noexcept final
  {
    return true;
  }

 private:
  void inputAttemptWhenModal() override { }
};

struct FakeScopedMessageLoopBlocker final :
    public ScopedMessageLoopBlocker
{
 public:
  inline FakeScopedMessageLoopBlocker() noexcept = default;
  inline ~FakeScopedMessageLoopBlocker() noexcept final = default;

  [[maybe_unused, nodiscard]] inline bool isReal() const noexcept final
  {
    return false;
  }
};


[[maybe_unused, nodiscard]] inline std::unique_ptr<ScopedMessageLoopBlocker>
blockMessageLoopInScopeIfNeeded(
    AbstractCoreContext& context,
    const JucePlugin&    plugin)
{
  if (needsMessageLoopBlocking(context, plugin))
    return std::make_unique<RealScopedMessageLoopBlocker>();

  return std::make_unique<FakeScopedMessageLoopBlocker>();
}

BLOOPER_UTIL_NAMESPACE_END

#endif // BLOOPER_MESSAGE_LOOP_BLOCKER_HPP
