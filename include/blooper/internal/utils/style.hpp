#ifndef BLOOPER_STYLE_UTILS_HPP
#define BLOOPER_STYLE_UTILS_HPP

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/abstract.hpp>
#include <blooper/internal/ext/ext.hpp>

BLOOPER_NAMESPACE_BEGIN

[[maybe_unused]] inline constexpr JuceColourId
    minimalBlooperColourId{100000000};

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
  outline [[maybe_unused]] = makeId(1001),

  cursor [[maybe_unused]] = makeId(1010),

  selection [[maybe_unused]] = makeId(1020),


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

    case ColourId::outline:
      return "Foreground";


    case ColourId::cursor:
      return "Cursor";


    case ColourId::selection:
      return "Selection";


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
  [[maybe_unused, nodiscard]] virtual bool
      hasColour(JuceColourId) = 0;

  [[maybe_unused, nodiscard]] virtual JuceColour
      getColour(JuceColourId) = 0;
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_STYLE_UTILS_HPP
