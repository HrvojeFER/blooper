#ifndef BLOOPER_EXT_STRING_HPP
#define BLOOPER_EXT_STRING_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/const.hpp>

BLOOPER_EXT_NAMESPACE_BEGIN

[[maybe_unused]] inline auto split(
    const juce::String& string,
    juce::juce_wchar    by)
{
  juce::StringArray result;

  auto end = string.getCharPointer();
  auto begin = end;

  while (!end.isEmpty())
  {
    if (*end == by)
    {
      result.add({begin, end});
      begin = ++end;
    }
    else
    {
      ++end;
    }
  }
  result.add({begin, end});

  return result;
}

[[maybe_unused]] inline auto splitPath(const juce::String& path)
{
  return split(path, pathSeparator);
}

[[maybe_unused]] inline auto toId(juce::StringRef ref)
{
  return JuceStateIdentifier(ref.operator const char*());
}

[[maybe_unused]] inline auto toVar(juce::StringRef ref)
{
  return JuceVar(ref.operator const char*());
}

BLOOPER_EXT_NAMESPACE_END

#endif // BLOOPER_EXT_STRING_HPP
