#ifndef BLOOPER_EXT_PLUGIN_HPP
#define BLOOPER_EXT_PLUGIN_HPP

#include <blooper/internal/macros/macros.hpp>

BLOOPER_EXT_NAMESPACE_BEGIN

#if JUCE_LINUX
constexpr bool shouldAddPluginWindowToDesktop = false;
#else
constexpr bool shouldAddPluginWindowToDesktop = true;
#endif

inline constexpr auto pluginIdSuffix = "_plugin";


[[maybe_unused]] inline juce::Identifier getId(
    const te::Plugin& plugin) noexcept
{
  return {plugin.itemID.toString() + pluginIdSuffix};
}

BLOOPER_EXT_NAMESPACE_END

#endif // BLOOPER_EXT_PLUGIN_HPP
