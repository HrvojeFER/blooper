#ifndef BLOOPER_EXT_WINDOW_HPP
#define BLOOPER_EXT_WINDOW_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/traits.hpp>

BLOOPER_EXT_NAMESPACE_BEGIN

template<typename TOnWindow>
[[maybe_unused]] inline void
visitWindows(TOnWindow onWindow) noexcept(noexcept(onWindow(
    std::declval<juce::TopLevelWindow*>())))
{
  static_assert(
      isInvokable(
          meta::typeid_(onWindow),
          meta::type_c<juce::TopLevelWindow*>),
      "onWindow passed to visitWindows must satisfy Invokable with "
      "juce::TopLevelWindow*.");

  for (int i = 0; i < juce::TopLevelWindow::getNumTopLevelWindows(); ++i)
  {
    onWindow(juce::TopLevelWindow::getTopLevelWindow(i));
  }
}

BLOOPER_EXT_NAMESPACE_END

#endif // BLOOPER_EXT_WINDOW_HPP
