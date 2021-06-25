#ifndef BLOOPER_EXT_GUI_HPP
#define BLOOPER_EXT_GUI_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/const.hpp>
#include <blooper/internal/abstract/juceTraits.hpp>

#include <blooper/internal/utils/ScopedMessageLoopBlocker.hpp>
#include <blooper/internal/utils/ScopedDPIAwarenessDisabler.hpp>
#include <blooper/internal/utils/style.hpp>

BLOOPER_UTIL_NAMESPACE_BEGIN

[[maybe_unused]] inline JuceBounds drawBottomLine(
    JuceGraphics&  g,
    JuceComponent& component,
    JuceBounds     availableArea)
{
  g.setColour(
      component.findColour(
          ColourId::outline));

  const auto availableWidth = availableArea.getWidth();
  const auto availableHeight = availableArea.getHeight();

  g.fillRect(
      availableWidth - outlineThickness * 2,
      availableHeight - outlineThickness * 2,
      availableWidth - outlineThickness * 2,
      outlineThickness);

  return availableArea.removeFromBottom(outlineThickness * 3);
}

[[maybe_unused]] inline JuceBounds drawBottomLine(
    JuceGraphics&  g,
    JuceComponent& component,
    JuceBounds     availableArea,
    bool           isSelected)
{
  if (isSelected)
  {
    g.setColour(
        component.findColour(
            ColourId::selection));
  }
  else
  {
    g.setColour(
        component.findColour(
            ColourId::outline));
  }

  const auto availableWidth = availableArea.getWidth();
  const auto availableHeight = availableArea.getHeight();

  g.fillRect(
      availableWidth - outlineThickness * 2,
      availableHeight - outlineThickness * 2,
      availableWidth - outlineThickness * 2,
      outlineThickness);

  return availableArea.removeFromBottom(outlineThickness * 3);
}

[[maybe_unused]] inline JuceBounds drawOutline(
    JuceGraphics&  g,
    JuceComponent& component,
    bool           isSelected)
{
  if (isSelected)
  {
    g.setColour(
        component.findColour(
            ColourId::selection));
  }
  else
  {
    g.setColour(
        component.findColour(
            ColourId::outline));
  }

  g.drawRect(
      component.getLocalBounds().reduced(outlineThickness),
      outlineThickness);

  return component.getLocalBounds().reduced(outlineThickness * 3);
}

[[maybe_unused]] inline JuceBounds drawOutline(
    JuceGraphics&  g,
    JuceComponent& component)
{
  g.setColour(
      component.findColour(
          ColourId::outline));

  g.drawRect(
      component.getLocalBounds().reduced(outlineThickness),
      outlineThickness);

  return component.getLocalBounds().reduced(outlineThickness * 3);
}

[[maybe_unused]] inline JuceBounds pad(
    const JuceBounds& area,
    int               byFactorOf = 1)
{
  return area.reduced(padding * byFactorOf);
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


[[maybe_unused, nodiscard]] inline std::pair<
    std::unique_ptr<ScopedMessageLoopBlocker>,
    std::unique_ptr<ScopedDPIAwarenessDisabler>>
declarePluginEditorCreationScope(
    AbstractCoreContext& context,
    const te::Plugin&    plugin)
{
  auto messageLoopBlocker =
      blockMessageLoopInScopeIfNeeded(context, plugin);

  auto dpiAwarenessDisabler =
      disableDPIInScopeIfNeeded(context, plugin);

  return {std::move(messageLoopBlocker),
          std::move(dpiAwarenessDisabler)};
}

BLOOPER_UTIL_NAMESPACE_END

#endif // BLOOPER_EXT_GUI_HPP
