#ifndef BLOOPER_SCOPED_DPI_AWARENESS_DISABLER_HPP
#define BLOOPER_SCOPED_DPI_AWARENESS_DISABLER_HPP

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/abstract.hpp>

BLOOPER_UTIL_NAMESPACE_BEGIN

inline bool needsDPIAwarenessDisabling(
    AbstractCoreContext& context,
    const te::Plugin&    plugin) noexcept
{
  // TODO: plugin DB
  return true;
}


struct ScopedDPIAwarenessDisabler
{
 public:
  inline ScopedDPIAwarenessDisabler() noexcept = default;
  inline virtual ~ScopedDPIAwarenessDisabler() noexcept = 0;

  [[maybe_unused, nodiscard]] inline virtual bool isReal() const noexcept = 0;
};

struct RealScopedDPIAwarenessDisabler final :
    public ScopedDPIAwarenessDisabler,
#if JUCE_WINDOWS && JUCE_WIN_PER_MONITOR_DPI_AWARE
    private juce::ScopedDPIAwarenessDisabler
#endif
{
 public:
  inline RealScopedDPIAwarenessDisabler() = default;
  inline ~RealScopedDPIAwarenessDisabler() final = default;

  [[maybe_unused, nodiscard]] inline bool isReal() const noexcept final
  {
    return true;
  }
};

struct FakeScopedDPIAwarenessDisabler final :
    ScopedDPIAwarenessDisabler
{
 public:
  inline FakeScopedDPIAwarenessDisabler() noexcept = default;
  inline ~FakeScopedDPIAwarenessDisabler() noexcept final = default;

  [[maybe_unused, nodiscard]] inline bool isReal() const noexcept final
  {
    return false;
  }
};


[[maybe_unused, nodiscard]] inline std::unique_ptr<ScopedDPIAwarenessDisabler>
disableDPIInScopeIfNeeded(
    AbstractCoreContext& context,
    const JucePlugin&    plugin)
{
  if (needsDPIAwarenessDisabling(context, plugin))
    return std::make_unique<RealScopedDPIAwarenessDisabler>();

  return std::make_unique<FakeScopedDPIAwarenessDisabler>();
}

BLOOPER_UTIL_NAMESPACE_END

#endif // BLOOPER_SCOPED_DPI_AWARENESS_DISABLER_HPP
