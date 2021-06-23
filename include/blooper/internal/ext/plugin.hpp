#ifndef BLOOPER_EXT_PLUGIN_HPP
#define BLOOPER_EXT_PLUGIN_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/const.hpp>

BLOOPER_EXT_NAMESPACE_BEGIN

[[maybe_unused]] inline juce::Identifier getId(
    const te::Plugin& plugin) noexcept
{
  return {plugin.itemID.toString() + pluginIdSuffix};
}

BLOOPER_EXT_NAMESPACE_END

#endif // BLOOPER_EXT_PLUGIN_HPP
