#ifndef BLOOPER_EXT_GUI_HPP
#define BLOOPER_EXT_GUI_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/const.hpp>
#include <blooper/internal/abstract/juceTraits.hpp>

#include <blooper/internal/utils/ScopedMessageLoopBlocker.hpp>
#include <blooper/internal/utils/ScopedDPIAwarenessDisabler.hpp>

BLOOPER_UTIL_NAMESPACE_BEGIN

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


[[maybe_unused]] inline JuceBounds drawBottomLine(
    JuceGraphics& g,
    JuceBounds    availableArea)
{
  const auto availableWidth = availableArea.getWidth();
  const auto availableHeight = availableArea.getHeight();

  g.fillRect(
      availableWidth - 4,
      availableHeight - 4,
      availableWidth - 4,
      2);

  return availableArea.removeFromBottom(lineSpace);
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
