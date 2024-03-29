#ifndef BLOOPER_UTILS_HPP
#define BLOOPER_UTILS_HPP

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/abstract.hpp>

#include <blooper/internal/utils/FlaggedAsyncUpdater.hpp>
#include <blooper/internal/utils/ScopedMessageLoopBlocker.hpp>
#include <blooper/internal/utils/ScopedDPIAwarenessDisabler.hpp>

#include <blooper/internal/utils/FilePathPropertyComponent.hpp>

#include <blooper/internal/utils/CoreContextFunctions.hpp>
#include <blooper/internal/utils/ContextFunctions.hpp>

#include <blooper/internal/utils/ContextCommands.hpp>

#include <blooper/internal/utils/EditTrack.hpp>

#include <blooper/internal/utils/selection.hpp>

#include <blooper/internal/utils/style.hpp>

#include <blooper/internal/utils/BarComponents.hpp>
#include <blooper/internal/utils/gui.hpp>

BLOOPER_UTIL_NAMESPACE_BEGIN

template<typename TCallback>
[[maybe_unused]] void requestRuntimePermissions(TCallback callback)
{
  static_assert(
      isInvokeable(meta::typeid_(callback), meta::type_c<bool>),
      "requestRuntimePermissions requires an Invokable with a bool.");


  juce::RuntimePermissions::request(
      juce::RuntimePermissions::recordAudio,
      callback);

  juce::RuntimePermissions::request(
      juce::RuntimePermissions::writeExternalStorage,
      callback);

  juce::RuntimePermissions::request(
      juce::RuntimePermissions::readExternalStorage,
      callback);
}


[[maybe_unused]] inline constexpr auto asyncCallbackDelayMilliseconds = 50;

template<typename TCallback>
[[maybe_unused]] void callAsync(TCallback callback)
{
  static_assert(
      isCallback(meta::typeid_(callback)),
      "callAsync requires a Callback.");

  juce::Timer::callAfterDelay(
      asyncCallbackDelayMilliseconds,
      move(callback));
}


[[maybe_unused]] inline void copyConstrainer(
    const JuceConstrainer& from,
    JuceConstrainer&       to) noexcept
{
  to.setSizeLimits(
      from.getMinimumWidth(),
      from.getMinimumHeight(),
      from.getMaximumWidth(),
      from.getMaximumHeight());

  to.setMinimumOnscreenAmounts(
      from.getMinimumWhenOffTheTop(),
      from.getMinimumWhenOffTheLeft(),
      from.getMinimumWhenOffTheBottom(),
      from.getMinimumWhenOffTheRight());

  to.setFixedAspectRatio(
      from.getFixedAspectRatio());
}

template<typename T>
[[maybe_unused]] inline constexpr auto toggle(T& _) noexcept
    -> decltype(auto)
{
  return _ = !_;
}

BLOOPER_UTIL_NAMESPACE_END

#endif // BLOOPER_UTILS_HPP
