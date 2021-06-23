#ifndef BLOOPER_CALLBACKS_HPP
#define BLOOPER_CALLBACKS_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/const.hpp>

BLOOPER_NAMESPACE_BEGIN

template<typename TCallback>
[[maybe_unused]] void requestRuntimePermissions(TCallback callback)
{
  static_assert(
      isInvokable(meta::typeid_(callback), meta::type_c<bool>),
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
[[maybe_unused]] void callDelayed(TCallback callback)
{
  static_assert(
      isCallback(meta::typeid_(callback)),
      "callDelayed requires a Callback.");

  juce::Timer::callAfterDelay(
      delayedCallbackDelayMilliseconds,
      move(callback));
}

BLOOPER_NAMESPACE_END

#endif // BLOOPER_CALLBACKS_HPP
