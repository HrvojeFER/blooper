#ifndef BLOOPER_ABSTRACT_CLIPS_HPP
#define BLOOPER_ABSTRACT_CLIPS_HPP

#include <blooper/internal/macros/macros.hpp>

#include <blooper/internal/abstract/prelude.hpp>
#include <blooper/internal/abstract/meta.hpp>
#include <blooper/internal/abstract/traits.hpp>
#include <blooper/internal/abstract/juceTraits.hpp>

#include <blooper/internal/abstract/stateful.hpp>
#include <blooper/internal/abstract/contextual.hpp>

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

[[maybe_unused]] inline constexpr auto isAnyClip =
    meta::satisfies_all(
        isJuceClip);

[[maybe_unused]] inline constexpr auto isAnyClipRef =
    meta::satisfies_all(
        isJuceClipRef);

[[maybe_unused]] inline constexpr auto isAnyClipConstRef =
    meta::satisfies_all(
        isJuceClipConstRef);


[[maybe_unused]] inline constexpr auto isAnyClipTraits =
    meta::attribute(
        [](auto&& toCheck)
            -> decltype(meta::and_(
                isAnyClip(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     clipType>),
                isAnyClipRef(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     clipRefType>),
                isAnyClipConstRef(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     clipConstRefType>))) {}) ^
    meta::after ^
    meta::check(
        [](auto&& toCheck)
            -> decltype(meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         clipType>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         clipRefType>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         clipConstRefType>) {});

template<typename TClip>
struct [[maybe_unused]] AnyClipTraits
{
  using clipType [[maybe_unused]] = TClip;

  using clipRefType [[maybe_unused]] =
      juce::ReferenceCountedObjectPtr<clipType>;

  // can't put const chere because of how juce does reference counting...
  using clipConstRefType [[maybe_unused]] =
      juce::ReferenceCountedObjectPtr<clipType>;
};


[[maybe_unused]] inline constexpr auto isAnyClipContentComponent =
    meta::satisfies_all(
        isAnyComponent,
        meta::attribute(
            [](auto&& toCheck)
                -> decltype(meta::and_(
                    isAnyClip(meta::typeid_(toCheck.getClip())),
                    isAnyClipRef(meta::typeid_(toCheck.getClipRef())))) {}) ^
            meta::after ^
            meta::check(
                [](auto&& toCheck)
                    -> decltype(toCheck.getClip(),
                                toCheck.getClipRef()) {}));

template<
    typename TClipContentComponentTraits,
    typename THeldClipTraits =
        typename TClipContentComponentTraits::clipTraits>
[[maybe_unused]] inline constexpr auto isAnyClipContentComponentBase =
    meta::satisfies_all(
        meta::reverse_partial(
            meta::traits::is_constructible,
            meta::type_c<typename TClipContentComponentTraits::
                             componentTraits::contextualTraits::contextType&>,
            meta::type_c<typename TClipContentComponentTraits::
                             componentTraits::statefulTraits::stateType>,
            meta::type_c<typename THeldClipTraits::clipRefType>),
        isAnyClipContentComponent);


[[maybe_unused]] inline constexpr auto isAnyClipContentTraits =
    meta::attribute(
        [](auto&& toCheck)
            -> decltype(meta::and_(
                isAnyClipTraits(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     clipTraits>),
                isAnyComponentTraits(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     componentTraits>),
                isAnyClipContentComponent(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     abstractType>),
                isAnyClipContentComponentBase<
                    std::decay_t<decltype(toCheck)>>(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     baseType>))) {}) ^
    meta::after ^
    meta::check(
        [](auto&& toCheck)
            -> decltype(meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         clipTraits>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         componentTraits>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         abstractType>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         baseType>) {});

template<typename TClipContentTraits>
class [[maybe_unused]] AnyAbstractClipContentComponent;

template<
    typename TClipContentTraits,
    typename THeldClip = typename TClipContentTraits::clipTraits>
class [[maybe_unused]] AnyClipContentComponentBase;

template<
    typename TClipTraits,
    typename TComponentTraits,
    typename = void>
struct [[maybe_unused]] AnyClipContentTraits
{
  using clipTraits [[maybe_unused]] = TClipTraits;

  static_assert(
      isAnyClipTraits(meta::type_c<clipTraits>),
      "AnyClipContentTraits requires AnyClipTraits.");

  using componentTraits [[maybe_unused]] = TComponentTraits;

  static_assert(
      isAnyComponentTraits(meta::type_c<componentTraits>),
      "AnyClipContentTraits requires AnyComponentTraits.");


  using abstractType [[maybe_unused]] =
      AnyAbstractClipContentComponent<
          AnyClipContentTraits>;

  using baseType [[maybe_unused]] =
      AnyClipContentComponentBase<
          AnyClipContentTraits>;
};


[[maybe_unused]] inline constexpr auto isAnyHeldClipContentTraits =
    meta::attribute(
        [](auto&& toCheck)
            -> decltype(meta::and_(
                isAnyClipContentTraits(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     contentTraits>),
                isAnyClipTraits(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     heldClipTraits>),
                isAnyClipContentComponentBase<
                    typename std::decay_t<decltype(toCheck)>::
                        contentTraits,
                    typename std::decay_t<decltype(toCheck)>::
                        heldClipTraits>(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     baseType>))) {}) ^
    meta::after ^
    meta::check(
        [](auto&& toCheck)
            -> decltype(meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         contentTraits>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         heldClipTraits>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         baseType>) {});

template<
    typename TClipContentTraits,
    typename THeldClipTraits,
    typename = void>
struct [[maybe_unused]] AnyHeldClipContentTraits
{
  using contentTraits = TClipContentTraits;

  static_assert(
      isAnyClipContentTraits(meta::type_c<contentTraits>),
      "AnyHeldClipContentTraits requires AnyClipContentTraits.");

  using heldClipTraits [[maybe_unused]] = THeldClipTraits;

  static_assert(
      isAnyClipTraits(meta::type_c<heldClipTraits>),
      "AnyHeldClipContentTraits requires AnyClipTraits.");


  using baseType [[maybe_unused]] =
      AnyClipContentComponentBase<
          contentTraits,
          heldClipTraits>;
};


template<typename TClipContentTraits>
class [[maybe_unused]] AnyAbstractClipContentComponent :
    public virtual TClipContentTraits::componentTraits::abstractType
{
  using abstractComponentType [[maybe_unused]] =
      typename TClipContentTraits::componentTraits::abstractType;

  static_assert(
      isAnyComponent(meta::type_c<abstractComponentType>),
      "AnyAbstractClipContentComponent requires AnyComponent.");


  using clipType [[maybe_unused]] =
      typename TClipContentTraits::clipTraits::clipType;

  static_assert(
      isAnyClip(meta::type_c<clipType>),
      "AnyAbstractClipContentComponent requires AnyClip.");

  using clipRefType [[maybe_unused]] =
      typename TClipContentTraits::clipTraits::clipRefType;

  static_assert(
      isAnyClipRef(meta::type_c<clipRefType>),
      "AnyAbstractClipContentComponent requires AnyClipRef.");

  using clipConstRefType [[maybe_unused]] =
      typename TClipContentTraits::clipTraits::clipConstRefType;

  static_assert(
      isAnyClipConstRef(meta::type_c<clipConstRefType>),
      "AnyAbstractClipContentComponent requires AnyClipConstRef.");


 public:
  [[maybe_unused]] inline AnyAbstractClipContentComponent() = default;

  [[maybe_unused]] inline ~AnyAbstractClipContentComponent() override = default;


  [[maybe_unused, nodiscard]] inline virtual const clipType&
  getClip() const noexcept = 0;

  [[maybe_unused, nodiscard]] inline virtual clipType&
  getClip() noexcept = 0;


  [[maybe_unused, nodiscard]] inline virtual clipConstRefType
  getClipRef() const noexcept = 0;

  [[maybe_unused, nodiscard]] inline virtual clipRefType
  getClipRef() noexcept = 0;
};

template<typename TClipContentTraits, typename THeldClipTraits>
class [[maybe_unused]] AnyClipContentComponentBase :
    public virtual TClipContentTraits::abstractType,
    public TClipContentTraits::componentTraits::baseType
{
  using abstractType [[maybe_unused]] =
      typename TClipContentTraits::abstractType;


  using componentBaseType [[maybe_unused]] =
      typename TClipContentTraits::componentTraits::baseType;

  static_assert(
      isAnyComponentBase<typename TClipContentTraits::componentTraits>(
          meta::type_c<componentBaseType>),
      "AnyClipContentComponentBase requires AnyComponentBase.");


  using contextType [[maybe_unused]] =
      typename TClipContentTraits::
          componentTraits::
              contextualTraits::
                  contextType;

  static_assert(
      isAnyContext(meta::type_c<contextType>),
      "AnyClipContentComponentBase requires AnyContext.");

  using stateType [[maybe_unused]] =
      typename TClipContentTraits::
          componentTraits::
              statefulTraits::
                  stateType;

  static_assert(
      isAnyState(meta::type_c<stateType>),
      "AnyClipContentComponentBase requires AnyState.");


  using clipType [[maybe_unused]] =
      typename TClipContentTraits::clipTraits::clipType;

  static_assert(
      isAnyClip(meta::type_c<clipType>),
      "AnyClipContentComponentBase requires AnyClip.");

  using clipRefType [[maybe_unused]] =
      typename TClipContentTraits::clipTraits::clipRefType;

  static_assert(
      isAnyClipRef(meta::type_c<clipRefType>),
      "AnyClipContentComponentBase requires AnyClipRef.");

  using clipConstRefType [[maybe_unused]] =
      typename TClipContentTraits::clipTraits::clipConstRefType;

  static_assert(
      isAnyClipConstRef(meta::type_c<clipConstRefType>),
      "AnyClipContentComponentBase requires AnyClipConstRef.");


  using heldClipType [[maybe_unused]] =
      typename THeldClipTraits::clipType;

  static_assert(
      meta::and_(
          isAnyClip(meta::type_c<heldClipType>),
          meta::traits::is_convertible(
              meta::type_c<heldClipType&>,
              meta::type_c<clipType&>)),
      "AnyClipContentComponentBase requires AnyClip for "
      "the held clip type that is convertible to clipType.");

  using heldClipRefType [[maybe_unused]] =
      typename THeldClipTraits::clipRefType;

  static_assert(
      meta::and_(
          isAnyClipRef(meta::type_c<heldClipRefType>),
          meta::traits::is_convertible(
              meta::type_c<heldClipRefType>,
              meta::type_c<clipRefType>)),
      "AnyClipContentComponentBase requires AnyClipRef for "
      "held clip ref type that is convertible to the clipRefType.");

  using heldClipConstRefType [[maybe_unused]] =
      typename THeldClipTraits::clipConstRefType;

  static_assert(
      meta::and_(
          isAnyClipConstRef(meta::type_c<heldClipConstRefType>),
          meta::traits::is_convertible(
              meta::type_c<heldClipConstRefType>,
              meta::type_c<clipConstRefType>)),
      "AnyClipContentComponentBase requires AnyClipConstRef for "
      "held clip const ref type that is convertible to the clipConstRefType.");


 public:
  [[maybe_unused]] explicit AnyClipContentComponentBase(
      contextType&    context,
      stateType       state,
      heldClipRefType clip)
      : componentBaseType(
            context,
            move(state)),
        clip(move(clip))
  {
    BLOOPER_ASSERT(this->clip);
  }


  [[maybe_unused, nodiscard]] inline const clipType&
  getClip() const noexcept final
  {
    return this->getHeldClip();
  }

  [[maybe_unused, nodiscard]] inline clipType&
  getClip() noexcept final
  {
    return this->getHeldClip();
  }

  [[maybe_unused, nodiscard]] inline clipConstRefType
  getClipRef() const noexcept final
  {
    return this->getHeldClipRef();
  }

  [[maybe_unused, nodiscard]] inline clipRefType
  getClipRef() noexcept final
  {
    return this->getHeldClipRef();
  }


 protected:
  [[maybe_unused, nodiscard]] inline const heldClipType&
  getHeldClip() const noexcept
  {
    return *this->clip;
  }

  [[maybe_unused, nodiscard]] inline heldClipType&
  getHeldClip() noexcept
  {
    return *this->clip;
  }

  [[maybe_unused, nodiscard]] inline heldClipConstRefType
  getHeldClipRef() const noexcept
  {
    return this->clip;
  }

  [[maybe_unused, nodiscard]] inline heldClipRefType
  getHeldClipRef() noexcept
  {
    return this->clip;
  }


 private:
  [[maybe_unused]] heldClipRefType clip;
};


using ClipTraits [[maybe_unused]] =
    AnyClipTraits<JuceClip>;

using Clip [[maybe_unused]] =
    ClipTraits::clipType;

using ClipRef [[maybe_unused]] =
    ClipTraits::clipRefType;

using ClipConstRef [[maybe_unused]] =
    ClipTraits::clipConstRefType;

BLOOPER_STATIC_ASSERT(
    isAnyClipTraits(meta::type_c<ClipTraits>),
    "ClipTraits must satisfy AnyClipTraits.");

BLOOPER_STATIC_ASSERT(
    isAnyClip(meta::type_c<Clip>),
    "Clip must satisfy AnyClip.");

BLOOPER_STATIC_ASSERT(
    isAnyClipRef(meta::type_c<ClipRef>),
    "ClipRef must satisfy AnyClipRef.");

BLOOPER_STATIC_ASSERT(
    isAnyClipConstRef(meta::type_c<ClipConstRef>),
    "ClipConstRef must satisfy AnyClipConstRef.");

using ClipContentTraits [[maybe_unused]] =
    AnyClipContentTraits<
        ClipTraits,
        CoreComponentTraits>;

using AbstractClipContentComponent [[maybe_unused]] =
    ClipContentTraits::abstractType;

using ClipContentComponentBase [[maybe_unused]] =
    ClipContentTraits::baseType;

BLOOPER_STATIC_ASSERT(
    isAnyClipContentTraits(
        meta::type_c<ClipContentTraits>),
    "ClipContentTraits must satisfy AnyClipContentTraits.");

BLOOPER_STATIC_ASSERT(
    isAnyClipContentComponent(
        meta::type_c<AbstractClipContentComponent>),
    "AbstractClipContentComponent must satisfy AnyClipContentComponent.");

BLOOPER_STATIC_ASSERT(
    isAnyClipContentComponentBase<ClipContentTraits>(
        meta::type_c<ClipContentComponentBase>),
    "ClipContentComponentBase must satisfy AnyClipContentComponentBase.");


// Concrete aliases

using AudioClipTraits [[maybe_unused]] =
    AnyClipTraits<JuceAudioClip>;

using AudioClip [[maybe_unused]] =
    AudioClipTraits::clipType;

using AudioClipRef [[maybe_unused]] =
    AudioClipTraits::clipRefType;

using AudioClipConstRef [[maybe_unused]] =
    AudioClipTraits::clipConstRefType;

using AudioClipContentTraits [[maybe_unused]] =
    AnyClipContentTraits<
        AudioClipTraits,
        CoreComponentTraits>;

using AbstractAudioClipContentComponent [[maybe_unused]] =
    AudioClipContentTraits::abstractType;

using AudioClipContentComponentBase [[maybe_unused]] =
    AudioClipContentTraits::baseType;


using WaveAudioClipTraits [[maybe_unused]] =
    AnyClipTraits<JuceWaveAudioClip>;

using WaveAudioClip [[maybe_unused]] =
    WaveAudioClipTraits::clipType;

using WaveAudioClipRef [[maybe_unused]] =
    WaveAudioClipTraits::clipRefType;

using WaveAudioClipConstRef [[maybe_unused]] =
    WaveAudioClipTraits::clipConstRefType;

using WaveAudioClipContentTraits [[maybe_unused]] =
    AnyClipContentTraits<
        WaveAudioClipTraits,
        CoreComponentTraits>;

using WaveAudioClipContentComponentBase [[maybe_unused]] =
    WaveAudioClipContentTraits::baseType;


using MidiClipTraits [[maybe_unused]] =
    AnyClipTraits<JuceMidiClip>;

using MidiClip [[maybe_unused]] =
    MidiClipTraits::clipType;

using MidiClipRef [[maybe_unused]] =
    MidiClipTraits::clipRefType;

using MidiClipConstRef [[maybe_unused]] =
    MidiClipTraits::clipConstRefType;

using MidiClipContentTraits [[maybe_unused]] =
    AnyClipContentTraits<
        MidiClipTraits,
        CoreComponentTraits>;

using MidiClipContentComponentBase [[maybe_unused]] =
    MidiClipContentTraits::baseType;


// Explicit instantiation

extern template class AnyAbstractClipContentComponent<ClipContentTraits>;
extern template class AnyClipContentComponentBase<ClipContentTraits>;

extern template class AnyAbstractClipContentComponent<AudioClipContentTraits>;
extern template class AnyClipContentComponentBase<AudioClipContentTraits>;

extern template class AnyClipContentComponentBase<WaveAudioClipContentTraits>;

extern template class AnyClipContentComponentBase<MidiClipContentTraits>;

BLOOPER_NAMESPACE_END

#endif // BLOOPER_ABSTRACT_CLIPS_HPP
