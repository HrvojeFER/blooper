#ifndef BLOOPER_STYLE_UTILS_HPP
#define BLOOPER_STYLE_UTILS_HPP
#pragma once

#include <blooper/internal/macros/namespaces.hpp>

#include <blooper/internal/abstract/const.hpp>
#include <blooper/internal/abstract/juceTraits.hpp>

BLOOPER_NAMESPACE_BEGIN

namespace ColourId
{
inline constexpr JuceColourId makeId(JuceColourId from) noexcept
{
  return from + minimalBlooperColourId;
}


// Taking inspiration from MS Terminal

enum _ : JuceColourId
{
  transparent [[maybe_unused]] = makeId(0),


  // Generic

  background [[maybe_unused]] = makeId(1000),
  foreground [[maybe_unused]] = makeId(1001),
  highlight [[maybe_unused]] = makeId(1002),

  outline [[maybe_unused]] = makeId(1010),
  selection [[maybe_unused]] = makeId(1011),

  cursor [[maybe_unused]] = makeId(1020),


  // Coloured

  white [[maybe_unused]] = makeId(2000),
  whiteBright [[maybe_unused]] = makeId(2001),

  black [[maybe_unused]] = makeId(2010),
  blackBright [[maybe_unused]] = makeId(2011),

  red [[maybe_unused]] = makeId(2020),
  redBright [[maybe_unused]] = makeId(2021),

  green [[maybe_unused]] = makeId(2030),
  greenBright [[maybe_unused]] = makeId(2031),

  yellow [[maybe_unused]] = makeId(2040),
  yellowBright [[maybe_unused]] = makeId(2041),

  blue [[maybe_unused]] = makeId(2050),
  blueBright [[maybe_unused]] = makeId(2051),

  purple [[maybe_unused]] = makeId(2060),
  purpleBright [[maybe_unused]] = makeId(2061),

  cyan [[maybe_unused]] = makeId(2070),
  cyanBright [[maybe_unused]] = makeId(2071),
};
} // namespace ColourId


// utils

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


[[maybe_unused]] inline constexpr auto getColourName(JuceColourId id)
{
  // TODO: JUCE colours

  switch (id)
  {
    case ColourId::transparent:
      return "Transparent";


      // Generic

    case ColourId::background:
      return "Background";

    case ColourId::foreground:
      return "Foreground";

    case ColourId::highlight:
      return "Highlight";


    case ColourId::outline:
      return "Outline";

    case ColourId::selection:
      return "Selection";


    case ColourId::cursor:
      return "Cursor";


      // Colours

    case ColourId::white:
      return "White";

    case ColourId::whiteBright:
      return "White Bright";


    case ColourId::black:
      return "Black";

    case ColourId::blackBright:
      return "Black Bright";


    case ColourId::red:
      return "Red";

    case ColourId::redBright:
      return "Red Bright";


    case ColourId::green:
      return "Green";

    case ColourId::greenBright:
      return "Green Bright";


    case ColourId::yellow:
      return "Yellow";

    case ColourId::yellowBright:
      return "Yellow Bright";


    case ColourId::blue:
      return "Blue";

    case ColourId::blueBright:
      return "Blue Bright";


    case ColourId::purple:
      return "Purple";

    case ColourId::purpleBright:
      return "Purple Bright";


    case ColourId::cyan:
      return "Cyan";

    case ColourId::cyanBright:
      return "Cyan Bright";


    default:
      return "none";
  }
}


// Theme

class AbstractTheme
{
 public:
  virtual ~AbstractTheme() = default;


  [[maybe_unused, nodiscard]] virtual bool
      hasColour(JuceColourId) const = 0;

  [[maybe_unused, nodiscard]] virtual JuceColour
      getColour(JuceColourId) const = 0;
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_STYLE_UTILS_HPP
