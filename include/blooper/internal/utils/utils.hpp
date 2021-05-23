#ifndef BLOOPER_UTILS_HPP
#define BLOOPER_UTILS_HPP

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/abstract.hpp>

#include <blooper/internal/utils/FlaggedAsyncUpdater.hpp>
#include <blooper/internal/utils/CoreContextFunctions.hpp>
#include <blooper/internal/utils/ContextFunctions.hpp>


BLOOPER_UTIL_NAMESPACE_BEGIN

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

BLOOPER_UTIL_NAMESPACE_END

#endif // BLOOPER_UTILS_HPP
