#ifndef BLOOPER_TIME_HPP
#define BLOOPER_TIME_HPP
#pragma once

#include <blooper/internal/macros/namespaces.hpp>

BLOOPER_NAMESPACE_BEGIN

using Token = int;
inline constexpr Token invalidToken = -1;

[[maybe_unused, nodiscard]] inline constexpr bool
isValid(Token token) noexcept
{
  return token != invalidToken;
}

[[maybe_unused, nodiscard]] inline Token
generateToken() noexcept
{
  static juce::Atomic<Token> currentValue = 0;

  currentValue += 1;
  return currentValue.get();
}


using Tick = unsigned char;

using HiResTick = juce::int64;

enum class Interval : Tick
{
  quarterOfBeat [[maybe_unused]] = 1,
  halfOfBeat [[maybe_unused]] = 2,
  beat [[maybe_unused]] = 4,
  twoBeats [[maybe_unused]] = 8,
  fourBeats [[maybe_unused]] = 16,
  eightBeats [[maybe_unused]] = 32,
  sixteenBeats [[maybe_unused]] = 64,
};

enum class Delay : Tick
{
  none [[maybe_unused]] = 0,
  nextQuarterOfBeat [[maybe_unused]] = 1,
  nextHalfOfBeat [[maybe_unused]] = 2,
  nextBeat [[maybe_unused]] = 4,
  nextSecondBeat [[maybe_unused]] = 8,
  nextFourthBeat [[maybe_unused]] = 16,
  nextEightBeat [[maybe_unused]] = 32,
  nextSixteenthBeat [[maybe_unused]] = 64,
};

[[maybe_unused, nodiscard]] inline constexpr double
getBeats(Interval of) noexcept
{
  return static_cast<double>(of) / 4.0;
}

[[maybe_unused, nodiscard]] inline constexpr double
getBeats(Delay of) noexcept
{
  return static_cast<double>(of) / 4.0;
}


enum class TrackMode : int
{
  sync [[maybe_unused]] = 1,
  oneShot [[maybe_unused]] = 2,
  free [[maybe_unused]] = 3,
};

BLOOPER_NAMESPACE_END

namespace juce
{
template<>
struct VariantConverter<blooper::Interval>
{
  static blooper::Interval fromVar(const var& v)
  {
    return static_cast<blooper::Interval>(static_cast<int>(v));
  }
  static var toVar(blooper::Interval i)
  {
    return static_cast<int>(i);
  }
};
template<>
struct VariantConverter<blooper::Delay>
{
  static blooper::Delay fromVar(const var& v)
  {
    return static_cast<blooper::Delay>(static_cast<int>(v));
  }
  static var toVar(blooper::Delay i)
  {
    return static_cast<int>(i);
  }
};

template<>
struct VariantConverter<blooper::TrackMode>
{
  static blooper::TrackMode fromVar(const var& v)
  {
    return static_cast<blooper::TrackMode>(static_cast<int>(v));
  }
  static var toVar(const blooper::TrackMode& i)
  {
    return static_cast<int>(i);
  }
};

template<>
struct VariantConverter<blooper::Tick>
{
  static blooper::Tick fromVar(const var& v)
  {
    return static_cast<blooper::Tick>(static_cast<int>(v));
  }
  static var toVar(blooper::Tick i)
  {
    return static_cast<int>(i);
  }
};
} // namespace juce

#endif //BLOOPER_TIME_HPP
