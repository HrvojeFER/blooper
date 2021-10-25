#ifndef BLOOPER_TIME_HPP
#define BLOOPER_TIME_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>

#include <blooper/internal/abstract/prelude.hpp>
#include <blooper/internal/abstract/meta.hpp>
#include <blooper/internal/abstract/traits.hpp>
#include <blooper/internal/abstract/juceTraits.hpp>

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

  return currentValue += 1;
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


class AbstractTimePixelConverter;

[[maybe_unused]] inline constexpr auto isAnyTimePixelConverter =
    meta::attribute(
        [](auto&& toCheck)
            -> decltype(meta::and_(
                meta::traits::is_convertible(
                    meta::typeid_(toCheck.getTimePixelMapping(
                        std::declval<AbstractTimePixelConverter*>())),
                    meta::type_c<TimePixelMapping>),
                meta::traits::is_convertible(
                    meta::typeid_(toCheck.getTimePixelMapping()),
                    meta::type_c<TimePixelMapping>))) {}) ^
    meta::after ^
    meta::check(
        [](auto&& toCheck)
            -> decltype(toCheck.getTimePixelMapping(
                            std::declval<AbstractTimePixelConverter*>()),
                        toCheck.getTimePixelMapping()) {});

class AbstractTimePixelConverter
{
 public:
  [[maybe_unused]] inline AbstractTimePixelConverter() = default;

  [[maybe_unused]] virtual inline ~AbstractTimePixelConverter() = default;


  [[maybe_unused, nodiscard]] virtual JuceTimeRange
  getAbsoluteTimeRange() const noexcept = 0;


  [[nodiscard]] virtual JuceTimeRange
  getTimeRange() const noexcept = 0;

  [[nodiscard]] virtual JucePixelRange
  getPixelRange() const noexcept = 0;


  [[nodiscard]] virtual JuceTimeRange
  getTimeRange(AbstractTimePixelConverter* in) const noexcept
  {
    if (!in) return this->getTimeRange();

    return in->getTimeRange().constrainRange(
        this->getTimeRange());
  }

  [[nodiscard]] virtual JucePixelRange
  getPixelRange(AbstractTimePixelConverter* in) const noexcept
  {
    if (!in) return this->getPixelRange();

    return in->getTimePixelMapping()
        .withTime(this->getTimeRange(in))
        .pixels;
  }


  [[nodiscard]] virtual TimePixelMapping
  getTimePixelMapping(AbstractTimePixelConverter* in) const
  {
    return {this->getTimeRange(),
            this->getPixelRange(in)};
  }

  [[nodiscard]] virtual TimePixelMapping
  getTimePixelMapping() const noexcept
  {
    return {this->getTimeRange(),
            this->getPixelRange()};
  }
};

BLOOPER_STATIC_ASSERT(
    isAnyTimePixelConverter(meta::type_c<AbstractTimePixelConverter>),
    "AbstractTimeXConverter must satisfy TimeXConverter.");


class [[maybe_unused]] AbstractTimePixelConverterComponent :
    public virtual JuceComponent,
    public virtual AbstractTimePixelConverter
{
 public:
  [[maybe_unused]] inline AbstractTimePixelConverterComponent() = default;

  [[maybe_unused]] virtual inline ~AbstractTimePixelConverterComponent() = default;


  [[maybe_unused, nodiscard]] JuceTimeRange
  getTimeRange() const noexcept override
  {
    auto parentConverter =
        dynamic_cast<AbstractTimePixelConverter*>(
            this->getParentComponent());

    if (!parentConverter) return this->getAbsoluteTimeRange();

    return parentConverter->getTimePixelMapping()
        .withPixels(this->getBounds().getHorizontalRange())
        .time.constrainRange(this->getAbsoluteTimeRange());
  }

  [[maybe_unused, nodiscard]] JucePixelRange
  getPixelRange() const noexcept override
  {
    auto parentConverter =
        dynamic_cast<AbstractTimePixelConverter*>(
            this->getParentComponent());

    if (!parentConverter) return this->getLocalBounds().getHorizontalRange();

    return parentConverter->getTimePixelMapping()
               .withTime(this->getTimeRange())
               .pixels -
           this->getBounds().getX();
  }

  [[maybe_unused, nodiscard]] JuceTimeRange
  getTimeRange(AbstractTimePixelConverter* in) const noexcept override
  {
    if (!in) return this->getTimeRange();

    return in->getTimeRange().constrainRange(
        this->getAbsoluteTimeRange());
  }
};

BLOOPER_STATIC_ASSERT(
    isAnyTimePixelConverter(meta::type_c<AbstractTimePixelConverterComponent>),
    "AbstractTimePixelConverterComponent must satisfy TimeXConverter.");

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

#endif // BLOOPER_TIME_HPP
