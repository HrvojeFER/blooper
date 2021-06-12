#ifndef BLOOPER_EXT_COMPONENT_HPP
#define BLOOPER_EXT_COMPONENT_HPP

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/abstract.hpp>

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

BLOOPER_EXT_NAMESPACE_END

#endif // BLOOPER_EXT_COMPONENT_HPP
