#include <blooper/context/style/DefaultTheme.hpp>

BLOOPER_NAMESPACE_BEGIN

bool DefaultTheme::hasColour(JuceColourId id) const
{
  return id == ColourId::transparent ||


         // Generic

         id == ColourId::background ||
         id == ColourId::foreground ||
         id == ColourId::highlight ||

         id == ColourId::outline ||
         id == ColourId::selection ||

         id == ColourId::cursor ||


         // Colours

         id == ColourId::white ||
         id == ColourId::whiteBright ||

         id == ColourId::black ||
         id == ColourId::blackBright ||

         id == ColourId::red ||
         id == ColourId::redBright ||

         id == ColourId::green ||
         id == ColourId::greenBright ||

         id == ColourId::yellow ||
         id == ColourId::yellowBright ||

         id == ColourId::blue ||
         id == ColourId::blueBright ||

         id == ColourId::purple ||
         id == ColourId::purpleBright ||

         id == ColourId::cyan ||
         id == ColourId::cyanBright;
}

JuceColour DefaultTheme::getColour(JuceColourId id) const
{
  switch (id)
  {
    // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::transparent:
      return JuceColour{};


      // Generic

      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::background:
      return JuceColour{0xff292d3e};

      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::foreground:
      return JuceColour{0xff2f3347};

      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::highlight:
      return JuceColour{0xff434f82};


      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::outline:
      return JuceColour{0xff8b91b5};

      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::selection:
      return JuceColour{0xffffcb6b};


      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::cursor:
      return JuceColour{0xffe6e7fe};


      // Colours

      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::white:
      return JuceColour{0xfff1f1f1};

      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::whiteBright:
      return JuceColour{0xffffffff};


      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::black:
      return JuceColour{0xff000000};

      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::blackBright:
      return JuceColour{0xff676e95};


      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::red:
      return JuceColour{0xffff5370};

      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::redBright:
      return JuceColour{0xffff5370};


      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::green:
      return JuceColour{0xffc3e88d};

      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::greenBright:
      return JuceColour{0xffc3e88d};

      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::yellow:
      return JuceColour{0xffffcb6b};

      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::yellowBright:
      return JuceColour{0xffffcb6b};


      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::blue:
      return JuceColour{0xff82aaff};

      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::blueBright:
      return JuceColour{0xff82aaff};


      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::purple:
      return JuceColour{0xffc792ea};

      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::purpleBright:
      return JuceColour{0xffc792ea};


      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::cyan:
      return JuceColour{0xff89ddff};

      // NOLINTNEXTLINE(bugprone-branch-clone)
    case ColourId::cyanBright:
      return JuceColour{0xff89ddff};


      // NOLINTNEXTLINE(bugprone-branch-clone)
    default:
      return JuceColour{0};
  }
}

BLOOPER_NAMESPACE_END
