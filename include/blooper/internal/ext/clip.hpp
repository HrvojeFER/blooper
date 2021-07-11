#ifndef BLOOPER_EXT_CLIP_HPP
#define BLOOPER_EXT_CLIP_HPP

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/predicates.hpp>
#include <blooper/internal/abstract/visitors.hpp>
#include <blooper/internal/abstract/const.hpp>
#include <blooper/internal/ext/take.hpp>

BLOOPER_EXT_NAMESPACE_BEGIN

// Types

[[maybe_unused]] inline te::WaveAudioClip*
isWaveClip(te::Clip* clip)
{
  return dynamic_cast<te::WaveAudioClip*>(clip);
}

[[maybe_unused]] inline const te::WaveAudioClip*
isWaveClip(const te::Clip* clip)
{
  return dynamic_cast<const te::WaveAudioClip*>(clip);
}

[[maybe_unused]] inline te::WaveAudioClip*
isWaveClip(te::Clip& clip)
{
  return dynamic_cast<te::WaveAudioClip*>(std::addressof(clip));
}

[[maybe_unused]] inline const te::WaveAudioClip*
isWaveClip(const te::Clip& clip)
{
  return dynamic_cast<const te::WaveAudioClip*>(std::addressof(clip));
}


[[maybe_unused]] inline te::MidiClip*
isMidiClip(te::Clip* clip)
{
  return dynamic_cast<te::MidiClip*>(clip);
}

[[maybe_unused]] inline const te::MidiClip*
isMidiClip(const te::Clip* clip)
{
  return dynamic_cast<const te::MidiClip*>(clip);
}

[[maybe_unused]] inline te::MidiClip*
isMidiClip(te::Clip& clip)
{
  return dynamic_cast<te::MidiClip*>(std::addressof(clip));
}

[[maybe_unused]] inline const te::MidiClip*
isMidiClip(const te::Clip& clip)
{
  return dynamic_cast<const te::MidiClip*>(std::addressof(clip));
}


// TODO: bump to track items

// Position

[[maybe_unused]] inline double getStart(te::Clip& clip)
{
  return clip.getPosition().getStart();
}

[[maybe_unused]] inline bool isOnLoopStart(te::Clip& clip)
{
  return getStart(clip) <= loopStartBeat;
}

[[maybe_unused]] inline double getEnd(te::Clip& clip)
{
  return clip.getPosition().getEnd();
}

[[maybe_unused]] inline bool isOnLoopEnd(te::Clip& clip)
{
  return getEnd(clip) >= loopEndBeat;
}

[[maybe_unused]] inline double getLength(te::Clip& clip)
{
  return clip.getPosition().getLength();
}


// Moving

[[maybe_unused]] inline void moveToLoopStartBeats(te::Clip& clip, double beats)
{
  auto& tempo = clip.getClipTrack()->edit.tempoSequence;
  clip.setPosition(
      {{loopStartBeat,
        tempo.beatsToTime(std::max(beats, loopEndBeat))}});
}

[[maybe_unused]] inline void moveToLoopStart(te::Clip& clip, double length)
{
  auto& tempo = clip.getClipTrack()->edit.tempoSequence;
  moveToLoopStartBeats(clip, tempo.timeToBeats(length));
}

[[maybe_unused]] inline void moveToLoopStart(te::Clip& clip)
{
  moveToLoopStartBeats(clip, clip.getLengthInBeats());
}


[[maybe_unused]] inline void setPosition(
    te::Clip& clip,
    double    start,
    double    end,
    double    offset = 0)
{
  clip.setPosition(
      {{start,
        end},
       offset});
}

[[maybe_unused]] inline void setPositionBeats(
    te::Clip& clip,
    double    startBeat,
    double    endBeat,
    double    offsetInBeats = 0)
{
  auto& tempo = clip.getClipTrack()->edit.tempoSequence;
  setPosition(
      clip,
      tempo.beatsToTime(startBeat),
      tempo.beatsToTime(endBeat),
      tempo.beatsToTime(offsetInBeats));
}


// Copy

// TODO: optimize?

[[maybe_unused]] inline te::Clip& copy(te::Clip& clip)
{
  auto& track = *clip.getClipTrack();

  auto copyState = clip.state.createCopy();

  track.edit.createNewItemID().writeID(copyState, nullptr);
  return *track.insertClipWithState(move(copyState));
}


// Visit

[[maybe_unused]] inline constexpr auto isWaveClipVisitor =
    isVisitorOf ^ meta::type_c<WaveAudioTakeRef>;

[[maybe_unused]] inline constexpr auto isMidiClipVisitor =
    isVisitorOf ^ meta::type_c<MidiTakeRef>;

[[maybe_unused]] inline constexpr auto isClipVisitor =
    meta::satisfies_any(isWaveClipVisitor, isMidiClipVisitor);

template<typename TVisitor>
[[maybe_unused]] inline bool visit(
    te::Clip& clip,
    TVisitor  visitor,
    bool      includeComps = false)
{
  [[maybe_unused]] constexpr auto visitorTypeid =
      BLOOPER_TYPEID(visitor);

  static_assert(
      isClipVisitor(visitorTypeid),
      "te::Clip visit requires a ClipVisitor");


  if constexpr (visitorTypeid ^ isWaveClipVisitor)
  {
    if (auto waveClip =
            dynamic_cast<te::WaveAudioClip*>(
                std::addressof(clip));
        waveClip->hasAnyTakes())
    {
      auto&      compManager = waveClip->getCompManager();
      const auto takes = waveClip->getTakes();

      for (int i = 0; compManager.getTotalNumTakes(); ++i)
        if (implies(includeComps, compManager.isTakeComp(i)))
          if (auto take = takes[i]; take.isValid())
            if (!callVisitor(
                    visitor,
                    WaveAudioTakeRef(waveClip, i, move(take))))
              return stopVisit;
    }
  }

  // NOLINTNEXTLINE(readability-misleading-indentation)
  if constexpr (visitorTypeid ^ isMidiClipVisitor)
  {
    if (auto midiClip =
            dynamic_cast<te::MidiClip*>(
                std::addressof(clip));
        midiClip->hasAnyTakes())
    {
      auto& compManager = midiClip->getCompManager();

      for (int i = 0; i < compManager.getTotalNumTakes(); ++i)
        if (implies(includeComps, compManager.isTakeComp(i)))
          if (auto take = midiClip->getTakeSequence(i))
            if (!callVisitor(
                    visitor,
                    MidiTakeRef(midiClip, i, move(take))))
              return stopVisit;
    }
  }

  // NOLINTNEXTLINE(readability-misleading-indentation)
  return continueVisit;
}


// Find

[[maybe_unused]] inline constexpr auto isWaveClipPredicate =
    isPredicateOf ^ meta::type_c<WaveAudioTakeRef>;

[[maybe_unused]] inline constexpr auto isMidiClipPredicate =
    isPredicateOf ^ meta::type_c<MidiTakeRef>;

[[maybe_unused]] inline constexpr auto isClipPredicate =
    meta::satisfies_some(isWaveClipPredicate, isMidiClipPredicate);

template<typename TPredicate>
[[maybe_unused]] inline auto find(
    te::Clip&  clip,
    TPredicate predicate,
    bool       includeComps = false)
{
  [[maybe_unused]] constexpr auto predicateTypeid =
      BLOOPER_TYPEID(predicate);

  static_assert(
      isClipPredicate(predicateTypeid),
      "te::Clip find requires a ClipPredicate");

  using takeType =
      decltype(meta::origin(
          meta::if_(
              predicateTypeid ^ isWaveClipPredicate,
              meta::type_c<WaveAudioTakeRef>,
              meta::type_c<MidiTakeRef>)));


  takeType result{};

  visit(
      clip,
      [&result,
       predicate = move(predicate)](
          const takeType& take) {
        if (predicate(take))
        {
          result = take;
          return stopVisit;
        }

        return continueVisit;
      },
      includeComps);


  return result;
}

//BLOOPER_STATIC_ASSERT(
//    meta::traits::result_of(
//        meta::typeid_([] {
//          return find(
//              std::declval<te::Clip&>(),
//              [](const WaveAudioTakeRef&) { return true; });
//        })) ^
//    meta::traits::is ^
//    meta::type_c<WaveAudioTakeRef>);
//
//BLOOPER_STATIC_ASSERT(
//    meta::traits::result_of(
//        meta::typeid_([] {
//          return find(
//              std::declval<te::Clip&>(),
//              [](const MidiTakeRef&) { return true; });
//        })) ^
//    meta::traits::is ^
//    meta::type_c<MidiTakeRef>);

BLOOPER_EXT_NAMESPACE_END

#endif // BLOOPER_EXT_CLIP_HPP
