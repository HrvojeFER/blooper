#ifndef BLOOPER_EXT_PLUGIN_HPP
#define BLOOPER_EXT_PLUGIN_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/const.hpp>
#include <blooper/internal/abstract/predicates.hpp>
#include <blooper/internal/abstract/visitors.hpp>

BLOOPER_EXT_NAMESPACE_BEGIN

[[maybe_unused]] inline juce::Identifier getId(
    const te::Plugin& plugin) noexcept
{
  return {plugin.itemID.toString() + pluginIdSuffix};
}


//// Visit
//
//[[maybe_unused]] inline constexpr auto isParameterVisitor =
//    isVisitorOf ^ meta::type_c<te::AutomatableParameter&>;

BLOOPER_EXT_NAMESPACE_END

#endif // BLOOPER_EXT_PLUGIN_HPP
