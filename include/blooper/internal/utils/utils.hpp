#ifndef BLOOPER_UTILS_HPP
#define BLOOPER_UTILS_HPP

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/abstract.hpp>

#include <blooper/internal/utils/FlaggedAsyncUpdater.hpp>
#include <blooper/internal/utils/ScopedMessageLoopBlocker.hpp>
#include <blooper/internal/utils/ScopedDPIAwarenessDisabler.hpp>

#include <blooper/internal/utils/CoreContextFunctions.hpp>
#include <blooper/internal/utils/ContextFunctions.hpp>


BLOOPER_UTIL_NAMESPACE_BEGIN


[[maybe_unused]] inline constexpr auto asyncCallbackDelayMilliseconds = 50;


template<typename TCallback>
void requestRuntimePermissions(TCallback callback)
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

template<typename TCallback>
void callAsync(TCallback callback)
{
  static_assert(
      isCallback(meta::typeid_(callback)),
      "callAsync requires a Callback.");

  juce::Timer::callAfterDelay(
      asyncCallbackDelayMilliseconds,
      std::move(callback));
}

BLOOPER_UTIL_NAMESPACE_END

#endif // BLOOPER_UTILS_HPP
