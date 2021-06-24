#ifndef BLOOPER_EXT_COMPONENT_HPP
#define BLOOPER_EXT_COMPONENT_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/meta.hpp>
#include <blooper/internal/abstract/juceTraits.hpp>

BLOOPER_EXT_NAMESPACE_BEGIN

template<typename... TComponents>
[[maybe_unused]] inline auto addAndMakeVisible(
    juce::Component& to,
    TComponents&... components)
    -> decltype((to.addAndMakeVisible(components), ...))
{
  return (to.addAndMakeVisible(components), ...);
}


[[maybe_unused]] inline auto setHeight(
    juce::Component& _for,
    int              height)
{
  auto bounds = _for.getBounds();
  bounds.setHeight(height);
  _for.setBounds(move(bounds));
}

[[maybe_unused]] inline auto setWidth(
    juce::Component& _for,
    int              width)
{
  auto bounds = _for.getBounds();
  bounds.setWidth(width);
  _for.setBounds(move(bounds));
}


template<typename TOnComponent>
[[maybe_unused]] inline void
visitComponents(TOnComponent onComponent) noexcept(noexcept(onComponent(
    std::declval<juce::Component*>())))
{
  static_assert(
      isInvokable(
          meta::typeid_(onComponent),
          meta::type_c<juce::Component*>),
      "onComponent passed to visitComponents must satisfy Invokable with "
      "juce::Component*.");

  for (int i = 0; i < JuceDesktop::getInstance().getNumComponents(); ++i)
  {
    onComponent(JuceDesktop::getInstance().getComponent(i));
  }
}

BLOOPER_EXT_NAMESPACE_END

#endif // BLOOPER_EXT_COMPONENT_HPP
