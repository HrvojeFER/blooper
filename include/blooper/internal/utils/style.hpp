#ifndef BLOOPER_STYLE_UTILS_HPP
#define BLOOPER_STYLE_UTILS_HPP

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/abstract.hpp>
#include <blooper/internal/ext/ext.hpp>

BLOOPER_UTIL_NAMESPACE_BEGIN

namespace ColourId
{
enum _ : JuceColourId
{
  none [[maybe_unused]] = 0,

  panelBorderId [[maybe_unused]] =
};
}

[[maybe_unused]] inline JuceColour findColour(
    const JuceLookAndFeel& from,
    JuceColourId           id)
{
  return from.findColour(static_cast<JuceColourId>(id));
}

[[maybe_unused]] inline JuceColour findColour(
    const JuceComponent& from,
    JuceColourId         id,
    bool                 inheritFromParent = false)
{
  return from.findColour(static_cast<JuceColourId>(id), inheritFromParent);
}

BLOOPER_UTIL_NAMESPACE_END

#endif // BLOOPER_STYLE_UTILS_HPP
