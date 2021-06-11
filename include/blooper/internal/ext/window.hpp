#ifndef BLOOPER_EXT_WINDOW_HPP
#define BLOOPER_EXT_WINDOW_HPP

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/abstract.hpp>

BLOOPER_EXT_NAMESPACE_BEGIN

template<typename TOnWindow>
[[maybe_unused]] void
visitWindows(TOnWindow onWindow) noexcept(noexcept(onWindow(
    std::declval<juce::TopLevelWindow*>())))
{
  static_assert(
      isInvokeable(
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
