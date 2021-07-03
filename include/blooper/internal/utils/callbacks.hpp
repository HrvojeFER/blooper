#ifndef BLOOPER_CALLBACKS_HPP
#define BLOOPER_CALLBACKS_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/const.hpp>

BLOOPER_UTIL_NAMESPACE_BEGIN

template<typename TCallback>
[[maybe_unused]] void requestRuntimePermissions(TCallback callback)
{
  static_assert(
      BLOOPER_TYPEID(callback) ^ isConsumerOf ^ meta::typeid_c<bool>,
      "requestRuntimePermissions requires a Consumer of bool");


  juce::RuntimePermissions::request(
      juce::RuntimePermissions::readExternalStorage,
      callback);

  juce::RuntimePermissions::request(
      juce::RuntimePermissions::writeExternalStorage,
      callback);

  juce::RuntimePermissions::request(
      juce::RuntimePermissions::recordAudio,
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

BLOOPER_UTIL_NAMESPACE_END

#endif // BLOOPER_CALLBACKS_HPP
