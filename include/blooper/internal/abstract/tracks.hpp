#ifndef BLOOPER_ABSTRACT_TRACKS_HPP
#define BLOOPER_ABSTRACT_TRACKS_HPP

#include <blooper/internal/macros/macros.hpp>

#include <blooper/internal/abstract/prelude.hpp>
#include <blooper/internal/abstract/meta.hpp>
#include <blooper/internal/abstract/traits.hpp>
#include <blooper/internal/abstract/juceTraits.hpp>

#include <blooper/internal/abstract/stateful.hpp>
#include <blooper/internal/abstract/contextual.hpp>

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

[[maybe_unused]] inline constexpr auto isAnyTrack =
    meta::satisfies_all(
        isJuceTrack);

[[maybe_unused]] inline constexpr auto isAnyTrackRef =
    meta::satisfies_all(
        isJuceTrackRef);

[[maybe_unused]] inline constexpr auto isAnyTrackConstRef =
    meta::satisfies_all(
        isJuceTrackConstRef);


[[maybe_unused]] inline constexpr auto isAnyTrackTraits =
    meta::attribute(
        [](auto&& toCheck)
            -> decltype(meta::and_(
                isAnyTrack(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     trackType>),
                isAnyTrackRef(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     trackRefType>),
                isAnyTrackConstRef(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     trackConstRefType>))) {}) ^
    meta::after ^
    meta::check(
        [](auto&& toCheck)
            -> decltype(meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         trackType>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         trackRefType>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         trackConstRefType>) {});

template<typename TTrack>
struct [[maybe_unused]] AnyTrackTraits
{
  using trackType [[maybe_unused]] = TTrack;

  using trackRefType [[maybe_unused]] =
      juce::ReferenceCountedObjectPtr<trackType>;

  // can't put const chere because of how juce does reference counting...
  using trackConstRefType [[maybe_unused]] =
      juce::ReferenceCountedObjectPtr<trackType>;
};


[[maybe_unused]] inline constexpr auto isAnyTrackContentComponent =
    meta::satisfies_all(
        isAnyComponent,
        meta::attribute(
            [](auto&& toCheck)
                -> decltype(meta::and_(
                    isAnyTrack(meta::typeid_(toCheck.getTrack())),
                    isAnyTrackRef(meta::typeid_(toCheck.getTrackRef())))) {}) ^
            meta::after ^
            meta::check(
                [](auto&& toCheck)
                    -> decltype(toCheck.getTrack(),
                                toCheck.getTrackRef()) {}));

template<
    typename TTrackContentComponentTraits,
    typename THeldTrackTraits =
        typename TTrackContentComponentTraits::trackTraits>
[[maybe_unused]] inline constexpr auto isAnyTrackContentComponentBase =
    meta::satisfies_all(
        meta::reverse_partial(
            meta::traits::is_constructible,
            meta::type_c<typename TTrackContentComponentTraits::
                             componentTraits::contextualTraits::contextType&>,
            meta::type_c<typename TTrackContentComponentTraits::
                             componentTraits::statefulTraits::stateType>,
            meta::type_c<typename THeldTrackTraits::trackRefType>),
        isAnyTrackContentComponent);


[[maybe_unused]] inline constexpr auto isAnyTrackContentTraits =
    meta::attribute(
        [](auto&& toCheck)
            -> decltype(meta::and_(
                isAnyTrackTraits(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     trackTraits>),
                isAnyComponentTraits(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     componentTraits>),
                isAnyTrackContentComponent(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     abstractType>),
                isAnyTrackContentComponentBase<
                    std::decay_t<decltype(toCheck)>>(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     baseType>))) {}) ^
    meta::after ^
    meta::check(
        [](auto&& toCheck)
            -> decltype(meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         trackTraits>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         componentTraits>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         abstractType>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         baseType>) {});

template<typename TTrackContentTraits>
class [[maybe_unused]] AnyAbstractTrackContentComponent;

template<
    typename TTrackContentTraits,
    typename THeldTrack = typename TTrackContentTraits::trackTraits>
class [[maybe_unused]] AnyTrackContentComponentBase;

template<
    typename TTrackTraits,
    typename TComponentTraits,
    typename = void>
struct [[maybe_unused]] AnyTrackContentTraits
{
  using trackTraits [[maybe_unused]] = TTrackTraits;

  static_assert(
      isAnyTrackTraits(meta::type_c<trackTraits>),
      "AnyTrackContentTraits requires AnyTrackTraits.");

  using componentTraits [[maybe_unused]] = TComponentTraits;

  static_assert(
      isAnyComponentTraits(meta::type_c<componentTraits>),
      "AnyTrackContentTraits requires AnyComponentTraits.");


  using abstractType [[maybe_unused]] =
      AnyAbstractTrackContentComponent<
          AnyTrackContentTraits>;

  using baseType [[maybe_unused]] =
      AnyTrackContentComponentBase<
          AnyTrackContentTraits>;
};


[[maybe_unused]] inline constexpr auto isAnyHeldTrackContentTraits =
    meta::attribute(
        [](auto&& toCheck)
            -> decltype(meta::and_(
                isAnyTrackContentTraits(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     contentTraits>),
                isAnyTrackTraits(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     heldTrackTraits>),
                isAnyTrackContentComponentBase<
                    typename std::decay_t<decltype(toCheck)>::
                        contentTraits,
                    typename std::decay_t<decltype(toCheck)>::
                        heldTrackTraits>(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     baseType>))) {}) ^
    meta::after ^
    meta::check(
        [](auto&& toCheck)
            -> decltype(meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         contentTraits>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         heldTrackTraits>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         baseType>) {});

template<
    typename TTrackContentTraits,
    typename THeldTrackTraits,
    typename = void>
struct [[maybe_unused]] AnyHeldTrackContentTraits
{
  using contentTraits = TTrackContentTraits;

  static_assert(
      isAnyTrackContentTraits(meta::type_c<contentTraits>),
      "AnyHeldTrackContentTraits requires AnyTrackContentTraits.");

  using heldTrackTraits [[maybe_unused]] = THeldTrackTraits;

  static_assert(
      isAnyTrackTraits(meta::type_c<heldTrackTraits>),
      "AnyHeldTrackContentTraits requires AnyTrackTraits.");


  using baseType [[maybe_unused]] =
      AnyTrackContentComponentBase<
          contentTraits,
          heldTrackTraits>;
};


template<typename TTrackContentTraits>
class [[maybe_unused]] AnyAbstractTrackContentComponent :
    public virtual TTrackContentTraits::componentTraits::abstractType
{
  using abstractComponentType [[maybe_unused]] =
      typename TTrackContentTraits::componentTraits::abstractType;

  static_assert(
      isAnyComponent(meta::type_c<abstractComponentType>),
      "AnyAbstractTrackContentComponent requires AnyComponent.");


  using trackType [[maybe_unused]] =
      typename TTrackContentTraits::trackTraits::trackType;

  static_assert(
      isAnyTrack(meta::type_c<trackType>),
      "AnyAbstractTrackContentComponent requires AnyTrack.");

  using trackRefType [[maybe_unused]] =
      typename TTrackContentTraits::trackTraits::trackRefType;

  static_assert(
      isAnyTrackRef(meta::type_c<trackRefType>),
      "AnyAbstractTrackContentComponent requires AnyTrackRef.");

  using trackConstRefType [[maybe_unused]] =
      typename TTrackContentTraits::trackTraits::trackConstRefType;

  static_assert(
      isAnyTrackConstRef(meta::type_c<trackConstRefType>),
      "AnyAbstractTrackContentComponent requires AnyTrackConstRef.");


 public:
  [[maybe_unused]] inline AnyAbstractTrackContentComponent() = default;

  [[maybe_unused]] inline ~AnyAbstractTrackContentComponent() override = default;


  [[maybe_unused, nodiscard]] inline virtual const trackType&
  getTrack() const noexcept = 0;

  [[maybe_unused, nodiscard]] inline virtual trackType&
  getTrack() noexcept = 0;


  [[maybe_unused, nodiscard]] inline virtual trackConstRefType
  getTrackRef() const noexcept = 0;

  [[maybe_unused, nodiscard]] inline virtual trackRefType
  getTrackRef() noexcept = 0;
};

template<typename TTrackContentTraits, typename THeldTrackTraits>
class [[maybe_unused]] AnyTrackContentComponentBase :
    public virtual TTrackContentTraits::abstractType,
    public TTrackContentTraits::componentTraits::baseType
{
  using abstractType [[maybe_unused]] =
      typename TTrackContentTraits::abstractType;


  using componentBaseType [[maybe_unused]] =
      typename TTrackContentTraits::componentTraits::baseType;

  static_assert(
      isAnyComponentBase<typename TTrackContentTraits::componentTraits>(
          meta::type_c<componentBaseType>),
      "AnyTrackContentComponentBase requires AnyComponentBase.");


  using contextType [[maybe_unused]] =
      typename TTrackContentTraits::
          componentTraits::
              contextualTraits::
                  contextType;

  static_assert(
      isAnyContext(meta::type_c<contextType>),
      "AnyTrackContentComponentBase requires AnyContext.");

  using stateType [[maybe_unused]] =
      typename TTrackContentTraits::
          componentTraits::
              statefulTraits::
                  stateType;

  static_assert(
      isAnyState(meta::type_c<stateType>),
      "AnyTrackContentComponentBase requires AnyState.");


  using trackType [[maybe_unused]] =
      typename TTrackContentTraits::trackTraits::trackType;

  static_assert(
      isAnyTrack(meta::type_c<trackType>),
      "AnyTrackContentComponentBase requires AnyTrack.");

  using trackRefType [[maybe_unused]] =
      typename TTrackContentTraits::trackTraits::trackRefType;

  static_assert(
      isAnyTrackRef(meta::type_c<trackRefType>),
      "AnyTrackContentComponentBase requires AnyTrackRef.");

  using trackConstRefType [[maybe_unused]] =
      typename TTrackContentTraits::trackTraits::trackConstRefType;

  static_assert(
      isAnyTrackConstRef(meta::type_c<trackConstRefType>),
      "AnyTrackContentComponentBase requires AnyTrackConstRef.");


  using heldTrackType [[maybe_unused]] =
      typename THeldTrackTraits::trackType;

  static_assert(
      meta::and_(
          isAnyTrack(meta::type_c<heldTrackType>),
          meta::traits::is_convertible(
              meta::type_c<heldTrackType&>,
              meta::type_c<trackType&>)),
      "AnyTrackContentComponentBase requires AnyTrack for "
      "the held track type that is convertible to trackType.");

  using heldTrackRefType [[maybe_unused]] =
      typename THeldTrackTraits::trackRefType;

  static_assert(
      meta::and_(
          isAnyTrackRef(meta::type_c<heldTrackRefType>),
          meta::traits::is_convertible(
              meta::type_c<heldTrackRefType>,
              meta::type_c<trackRefType>)),
      "AnyTrackContentComponentBase requires AnyTrackRef for "
      "held track ref type that is convertible to the trackRefType.");

  using heldTrackConstRefType [[maybe_unused]] =
      typename THeldTrackTraits::trackConstRefType;

  static_assert(
      meta::and_(
          isAnyTrackConstRef(meta::type_c<heldTrackConstRefType>),
          meta::traits::is_convertible(
              meta::type_c<heldTrackConstRefType>,
              meta::type_c<trackConstRefType>)),
      "AnyTrackContentComponentBase requires AnyTrackConstRef for "
      "held track const ref type that is convertible to the trackConstRefType.");


 public:
  [[maybe_unused]] explicit AnyTrackContentComponentBase(
      contextType&     context,
      stateType        state,
      heldTrackRefType track)
      : componentBaseType(
            context,
            move(state)),
        track(move(track))
  {
    BLOOPER_ASSERT(this->track);
  }


  [[maybe_unused, nodiscard]] inline const trackType&
  getTrack() const noexcept final
  {
    return this->getHeldTrack();
  }

  [[maybe_unused, nodiscard]] inline trackType&
  getTrack() noexcept final
  {
    return this->getHeldTrack();
  }


  [[maybe_unused, nodiscard]] inline trackConstRefType
  getTrackRef() const noexcept final
  {
    return this->getHeldTrackRef();
  }

  [[maybe_unused, nodiscard]] inline trackRefType
  getTrackRef() noexcept final
  {
    return this->getHeldTrackRef();
  }


 protected:
  [[maybe_unused, nodiscard]] inline const heldTrackType&
  getHeldTrack() const noexcept
  {
    return *this->track;
  }

  [[maybe_unused, nodiscard]] inline heldTrackType&
  getHeldTrack() noexcept
  {
    return *this->track;
  }

  [[maybe_unused, nodiscard]] inline heldTrackConstRefType
  getHeldTrackRef() const noexcept
  {
    return this->track;
  }

  [[maybe_unused, nodiscard]] inline heldTrackRefType
  getHeldTrackRef() noexcept
  {
    return this->track;
  }


 private:
  [[maybe_unused]] heldTrackRefType track;
};


using TrackTraits [[maybe_unused]] =
    AnyTrackTraits<JuceTrack>;

using Track [[maybe_unused]] =
    TrackTraits::trackType;

using TrackRef [[maybe_unused]] =
    TrackTraits::trackRefType;

using TrackConstRef [[maybe_unused]] =
    TrackTraits::trackConstRefType;

BLOOPER_STATIC_ASSERT(
    isAnyTrackTraits(meta::type_c<TrackTraits>),
    "TrackTraits must satisfy AnyTrackTraits.");

BLOOPER_STATIC_ASSERT(
    isAnyTrack(meta::type_c<Track>),
    "Track must satisfy AnyTrack.");

BLOOPER_STATIC_ASSERT(
    isAnyTrackRef(meta::type_c<TrackRef>),
    "TrackRef must satisfy AnyTrackRef.");

BLOOPER_STATIC_ASSERT(
    isAnyTrackConstRef(meta::type_c<TrackConstRef>),
    "TrackConstRef must satisfy AnyTrackConstRef.");

using TrackContentTraits [[maybe_unused]] =
    AnyTrackContentTraits<
        TrackTraits,
        CoreComponentTraits>;

using AbstractTrackContentComponent [[maybe_unused]] =
    TrackContentTraits::abstractType;

using TrackContentComponentBase [[maybe_unused]] =
    TrackContentTraits::baseType;

BLOOPER_STATIC_ASSERT(
    isAnyTrackContentTraits(
        meta::type_c<TrackContentTraits>),
    "TrackContentTraits must satisfy AnyTrackContentTraits.");

BLOOPER_STATIC_ASSERT(
    isAnyTrackContentComponent(
        meta::type_c<AbstractTrackContentComponent>),
    "AbstractTrackContentComponent must satisfy AnyTrackContentComponent.");

BLOOPER_STATIC_ASSERT(
    isAnyTrackContentComponentBase<TrackContentTraits>(
        meta::type_c<TrackContentComponentBase>),
    "TrackContentComponentBase must satisfy AnyTrackContentComponentBase.");


using AudioTrackTraits [[maybe_unused]] =
    AnyTrackTraits<JuceAudioTrack>;

using AudioTrack [[maybe_unused]] =
    AudioTrackTraits::trackType;

using AudioTrackRef [[maybe_unused]] =
    AudioTrackTraits::trackRefType;

using AudioTrackConstRef [[maybe_unused]] =
    AudioTrackTraits::trackConstRefType;

BLOOPER_STATIC_ASSERT(
    isAnyTrackTraits(meta::type_c<AudioTrackTraits>),
    "AudioTrackTraits must satisfy AnyTrackTraits.");

BLOOPER_STATIC_ASSERT(
    isAnyTrack(meta::type_c<AudioTrack>),
    "AudioTrack must satisfy AnyTrack.");

BLOOPER_STATIC_ASSERT(
    isAnyTrackRef(meta::type_c<AudioTrackRef>),
    "AudioTrackRef must satisfy AnyTrackRef.");

BLOOPER_STATIC_ASSERT(
    isAnyTrackConstRef(meta::type_c<AudioTrackConstRef>),
    "AudioTrackConstRef must satisfy AnyTrackConstRef.");

using AudioTrackContentTraits [[maybe_unused]] =
    AnyTrackContentTraits<
        AudioTrackTraits,
        CoreComponentTraits>;

using AbstractAudioTrackContentComponent [[maybe_unused]] =
    AudioTrackContentTraits::abstractType;

using AudioTrackContentComponentBase [[maybe_unused]] =
    AudioTrackContentTraits::baseType;

BLOOPER_STATIC_ASSERT(
    isAnyTrackContentTraits(
        meta::type_c<AudioTrackContentTraits>),
    "AudioTrackContentTraits must satisfy AnyTrackContentTraits.");

BLOOPER_STATIC_ASSERT(
    isAnyTrackContentComponent(
        meta::type_c<AbstractAudioTrackContentComponent>),
    "AbstractAudioTrackContentComponent must satisfy AnyTrackContentComponent.");

BLOOPER_STATIC_ASSERT(
    isAnyTrackContentComponentBase<AudioTrackContentTraits>(
        meta::type_c<AudioTrackContentComponentBase>),
    "AudioTrackContentComponentBase must satisfy AnyTrackContentComponentBase.");


// Explicit instantiation

extern template class AnyAbstractTrackContentComponent<
    TrackContentTraits>;

extern template class AnyTrackContentComponentBase<
    TrackContentTraits,
    TrackTraits>;


extern template class AnyTrackContentComponentBase<
    TrackContentTraits,
    AudioTrackTraits>;


BLOOPER_NAMESPACE_END

#endif // BLOOPER_ABSTRACT_TRACKS_HPP
