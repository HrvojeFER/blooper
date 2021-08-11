#ifndef BLOOPER_ABSTRACT_TAKES_HPP
#define BLOOPER_ABSTRACT_TAKES_HPP

#include <blooper/internal/macros/macros.hpp>

#include <blooper/internal/abstract/prelude.hpp>
#include <blooper/internal/abstract/meta.hpp>
#include <blooper/internal/abstract/traits.hpp>
#include <blooper/internal/abstract/juceTraits.hpp>

#include <blooper/internal/abstract/stateful.hpp>
#include <blooper/internal/abstract/contextual.hpp>

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

// TODO: take functions into cpp file

// Takes

class AbstractTakeRef
{
 public:
  [[nodiscard]] inline virtual bool isValid() const noexcept = 0;

  [[maybe_unused, nodiscard]] inline virtual bool isInvalid() const noexcept
  {
    return !this->isValid();
  }


  [[nodiscard]] inline virtual bool isComp() const noexcept = 0;

  [[maybe_unused, nodiscard]] inline virtual bool isNotComp() const noexcept
  {
    return !this->isComp();
  }


  [[nodiscard]] inline virtual const te::Clip& getClip() const noexcept = 0;

  [[nodiscard]] inline virtual te::Clip& getClip() noexcept = 0;


  [[nodiscard]] inline virtual int getIndex() const noexcept = 0;


  [[maybe_unused]] virtual juce::ReferenceCountedObjectPtr<te::Clip>
  unpackInto(te::ClipTrack&) const = 0;


 protected:
  inline AbstractTakeRef() noexcept = default;

  virtual ~AbstractTakeRef() noexcept = default;
};

template<typename TClip>
class TakeRefBase :
    public virtual AbstractTakeRef
{
  static_assert(
      meta::type_c<TClip&> ^
          meta::traits::is_convertible_to ^
          meta::type_c<te::Clip&>,
      "clip passed to TakeRefBase has to be convertible to te::Clip&");


 public:
  TakeRefBase() noexcept = default;

  TakeRefBase(
      TClip* clip,
      int    index) noexcept
      : clip(clip),
        index(index)
  {
  }

  [[maybe_unused]] TClip* clip{nullptr};

  [[maybe_unused]] int index{-1};


  [[nodiscard]] inline bool isValid() const noexcept override
  {
    return this->clip != nullptr;
  }

  [[nodiscard]] inline bool isInvalid() const noexcept override
  {
    return this->clip == nullptr;
  }


  [[nodiscard]] inline const te::Clip& getClip() const noexcept final
  {
    return *this->clip;
  }

  [[nodiscard]] inline te::Clip& getClip() noexcept final
  {
    return *this->clip;
  }


  [[nodiscard]] inline int getIndex() const noexcept final
  {
    return this->index;
  }
};

class [[maybe_unused]] TakeRef final :
    public virtual AbstractTakeRef,
    public TakeRefBase<te::Clip>
{
 public:
  TakeRef() noexcept = default;

  TakeRef(
      te::AudioClipBase* clip,
      int                index) noexcept
      : TakeRefBase(
            clip,
            index)
  {
  }

  explicit TakeRef(
      AbstractTakeRef& other) noexcept
      : TakeRefBase(
            other.isValid() ?
                std::addressof(other.getClip()) :
                nullptr,
            other.getIndex())
  {
  }

  explicit TakeRef(
      AbstractTakeRef&& other) noexcept
      : TakeRefBase(
            other.isValid() ?
                std::addressof(other.getClip()) :
                nullptr,
            other.getIndex())
  {
  }


  [[nodiscard]] inline bool isValid() const noexcept final
  {
    return TakeRefBase::isValid();
  }

  [[nodiscard]] inline bool isInvalid() const noexcept final
  {
    return TakeRefBase::isInvalid();
  }


  [[nodiscard]] inline bool isComp() const noexcept final
  {
    if (auto waveClip = dynamic_cast<te::WaveAudioClip*>(this->clip))
    {
      return waveClip->getCompManager().isTakeComp(this->index);
    }

    if (auto midiClip = dynamic_cast<te::MidiClip*>(this->clip))
    {
      return midiClip->getCompManager().isTakeComp(this->index);
    }

    return false;
  }


  juce::ReferenceCountedObjectPtr<te::Clip>
  unpackInto(te::ClipTrack& track) const final
  {
    if (this->isComp()) return {};

    const bool shouldBeShowingTakes = this->clip->isShowingTakes();
    if (shouldBeShowingTakes)
      this->clip->setShowingTakes(false);


    juce::ReferenceCountedObjectPtr<te::Clip> insertedClip{};

    if (auto waveClip = dynamic_cast<te::WaveAudioClip*>(this->clip))
    {
      auto insertedWaveClip =
          track.insertWaveClip(
              waveClip->getTakeDescriptions()[this->index],
              waveClip->getTakes()[this->index],
              {{0.0,
                waveClip->getPosition().getLength()},
               waveClip->getPosition().getOffset()},
              false);

      if (auto takeTree = waveClip->state.getChildWithName(te::IDs::TAKES);
          takeTree.isValid())
        takeTree.removeChild(this->index, nullptr);

      insertedClip = move(insertedWaveClip);
    }

    if (auto midiClip = dynamic_cast<te::MidiClip*>(this->clip))
    {
      auto insertedMidiClip =
          track.insertMIDIClip(
              midiClip->getTakeDescriptions()[this->index],
              {{0.0,
                midiClip->getPosition().getLength()}},
              nullptr);

      if (this->isValid())
        insertedMidiClip->getSequence()
            .copyFrom(
                *midiClip->getTakeSequence(this->index),
                nullptr);

      if (auto takeTree = midiClip->state.getChildWithName(te::IDs::TAKES);
          takeTree.isValid())
        takeTree.removeChild(this->index, nullptr);

      insertedClip = move(insertedMidiClip);
    }


    if (shouldBeShowingTakes)
      this->clip->setShowingTakes(true);

    return move(insertedClip);
  }
};

template<typename TClip = te::AudioClipBase>
class [[maybe_unused]] AudioTakeRef :
    public virtual AbstractTakeRef,
    public TakeRefBase<TClip>
{
  using base = TakeRefBase<TClip>;

 public:
  AudioTakeRef() noexcept = default;

  AudioTakeRef(
      TClip*            clip,
      int               index,
      te::ProjectItemID audioFileId) noexcept
      : base(clip, index),
        audioFileId(move(audioFileId))
  {
  }

  te::ProjectItemID audioFileId{};


  [[nodiscard]] inline bool isValid() const noexcept
  {
    return base::isValid() && this->audioFileId.isValid();
  }

  [[nodiscard]] inline bool isInvalid() const noexcept
  {
    return base::isInvalid() && this->audioFileId.isInvalid();
  }


  [[nodiscard]] inline bool isComp() const noexcept
  {
    if (auto waveClip = dynamic_cast<te::WaveAudioClip*>(this->clip))
    {
      return waveClip->getCompManager().isTakeComp(this->index);
    }

    return false;
  }


  juce::ReferenceCountedObjectPtr<te::Clip>
  unpackInto(te::ClipTrack& track) const
  {
    if (!dynamic_cast<te::WaveAudioClip*>(this->clip)) return {};
    if (this->isComp()) return {};


    const bool shouldBeShowingTakes = this->clip->isShowingTakes();
    if (shouldBeShowingTakes)
      this->clip->setShowingTakes(false);


    auto insertedClip =
        track.insertWaveClip(
            this->clip->getTakeDescriptions()[this->index],
            this->audioFileId,
            {{0.0,
              this->clip->getPosition().getLength()},
             this->clip->getPosition().getOffset()},
            false);

    if (auto takeTree = this->clip->state.getChildWithName(te::IDs::TAKES);
        takeTree.isValid())
      takeTree.removeChild(this->index, nullptr);


    if (shouldBeShowingTakes)
      this->clip->setShowingTakes(true);

    return move(insertedClip);
  }
};

class [[maybe_unused]] WaveAudioTakeRef final :
    public virtual AbstractTakeRef,
    public AudioTakeRef<te::WaveAudioClip>
{
};

class [[maybe_unused]] MidiTakeRef final :
    public virtual AbstractTakeRef,
    public TakeRefBase<te::MidiClip>
{
 public:
  MidiTakeRef() noexcept = default;

  MidiTakeRef(
      te::MidiClip* clip,
      int           index,
      te::MidiList* midi) noexcept
      : TakeRefBase(clip, index),
        midi(move(midi))
  {
  }

  te::MidiList* midi{nullptr};


  [[nodiscard]] inline bool isValid() const noexcept final
  {
    return TakeRefBase::isValid() && this->midi != nullptr;
  }

  [[nodiscard]] inline bool isInvalid() const noexcept final
  {
    return TakeRefBase::isInvalid() && this->midi == nullptr;
  }


  [[nodiscard]] inline bool isComp() const noexcept final
  {
    return this->clip->getCompManager().isTakeComp(this->index);
  }


  juce::ReferenceCountedObjectPtr<te::Clip>
  unpackInto(te::ClipTrack& track) const final
  {
    if (this->isComp()) return nullptr;

    const bool shouldBeShowingTakes = this->clip->isShowingTakes();
    if (shouldBeShowingTakes)
      this->clip->setShowingTakes(false);


    auto insertedClip =
        track.insertMIDIClip(
            this->clip->getTakeDescriptions()[this->index],
            {{0.0,
              this->clip->getPosition().getLength()}},
            nullptr);

    if (this->isValid())
      insertedClip->getSequence()
          .copyFrom(
              *this->midi,
              nullptr);

    if (auto takeTree = this->clip->state.getChildWithName(te::IDs::TAKES);
        takeTree.isValid())
      takeTree.removeChild(this->index, nullptr);


    if (shouldBeShowingTakes)
      this->clip->setShowingTakes(true);

    return move(insertedClip);
  }
};


// Abstract

[[maybe_unused]] inline constexpr auto isAnyTakeRef =
    meta::traits::is_convertible_to ^ meta::type_c<AbstractTakeRef&&>;

[[maybe_unused]] inline constexpr auto isAnyTakeConstRef =
    meta::traits::is_convertible_to ^ meta::type_c<const AbstractTakeRef&&>;


[[maybe_unused]] inline constexpr auto isAnyTakeTraits =
    meta::attribute(
        [](auto&& toCheck)
            -> decltype(meta::and_(
                isAnyTakeRef(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     takeRefType>),
                isAnyTakeConstRef(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     takeConstRefType>))) {}) ^
    meta::after ^
    meta::check(
        [](auto&& toCheck)
            -> decltype(meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         takeRefType>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         takeConstRefType>) {});

template<typename TTakeRef>
struct [[maybe_unused]] AnyTakeTraits
{
  using takeRefType [[maybe_unused]] = TTakeRef;
  using takeConstRefType [[maybe_unused]] = const TTakeRef;
};


[[maybe_unused]] inline constexpr auto isAnyTakeContentComponent =
    meta::satisfies_all(
        isAnyComponent,
        meta::attribute(
            [](auto&& toCheck)
                -> decltype(meta::and_(
                    isAnyTakeRef(meta::typeid_(toCheck.getTakeRef())))) {}) ^
            meta::after ^
            meta::check(
                [](auto&& toCheck)
                    -> decltype(toCheck.getTakeRef()) {}));

template<
    typename TTakeContentComponentTraits,
    typename THeldTakeTraits>
[[maybe_unused]] inline constexpr auto isAnyTakeContentComponentBase =
    meta::satisfies_all(
        meta::reverse_partial(
            meta::traits::is_constructible,
            meta::type_c<typename TTakeContentComponentTraits::
                             componentTraits::contextualTraits::contextType&>,
            meta::type_c<typename TTakeContentComponentTraits::
                             componentTraits::statefulTraits::stateType>,
            meta::type_c<typename THeldTakeTraits::takeRefType>),
        isAnyTakeContentComponent);


[[maybe_unused]] inline constexpr auto isAnyTakeContentTraits =
    meta::attribute(
        [](auto&& toCheck)
            -> decltype(meta::and_(
                isAnyTakeTraits(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     takeTraits>),
                isAnyComponentTraits(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     componentTraits>),
                isAnyTakeContentComponent(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     abstractType>))) {}) ^
    meta::after ^
    meta::check(
        [](auto&& toCheck)
            -> decltype(meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         takeTraits>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         componentTraits>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         abstractType>) {});

template<typename TTakeContentTraits>
class [[maybe_unused]] AnyAbstractTakeContentComponent;

template<
    typename TTakeContentTraits,
    typename THeldTakeTraits>
class [[maybe_unused]] AnyTakeContentComponentBase;

template<
    typename TTakeTraits,
    typename TComponentTraits,
    typename = void>
struct [[maybe_unused]] AnyTakeContentTraits
{
  using takeTraits [[maybe_unused]] = TTakeTraits;

  static_assert(
      isAnyTakeTraits(meta::type_c<takeTraits>),
      "AnyTakeContentTraits requires AnyTakeTraits.");

  using componentTraits [[maybe_unused]] = TComponentTraits;

  static_assert(
      isAnyComponentTraits(meta::type_c<componentTraits>),
      "AnyTakeContentTraits requires AnyComponentTraits.");


  using abstractType [[maybe_unused]] =
      AnyAbstractTakeContentComponent<
          AnyTakeContentTraits>;
};


[[maybe_unused]] inline constexpr auto isAnyHeldTakeContentTraits =
    meta::attribute(
        [](auto&& toCheck)
            -> decltype(meta::and_(
                isAnyTakeContentTraits(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     contentTraits>),
                isAnyTakeTraits(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     heldTakeTraits>),
                isAnyTakeContentComponentBase<
                    typename std::decay_t<decltype(toCheck)>::
                        contentTraits,
                    typename std::decay_t<decltype(toCheck)>::
                        heldTakeTraits>(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     baseType>))) {}) ^
    meta::after ^
    meta::check(
        [](auto&& toCheck)
            -> decltype(meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         contentTraits>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         heldTakeTraits>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         baseType>) {});

template<
    typename TTakeContentTraits,
    typename THeldTakeTraits,
    typename = void>
struct [[maybe_unused]] AnyHeldTakeContentTraits
{
  using contentTraits = TTakeContentTraits;

  static_assert(
      isAnyTakeContentTraits(meta::type_c<contentTraits>),
      "AnyHeldTakeContentTraits requires AnyTakeContentTraits.");

  using heldTakeTraits [[maybe_unused]] = THeldTakeTraits;

  static_assert(
      isAnyTakeTraits(meta::type_c<heldTakeTraits>),
      "AnyHeldTakeContentTraits requires AnyTakeTraits.");


  using baseType [[maybe_unused]] =
      AnyTakeContentComponentBase<
          contentTraits,
          heldTakeTraits>;
};


template<typename TTakeContentTraits>
class [[maybe_unused]] AnyAbstractTakeContentComponent :
    public virtual TTakeContentTraits::componentTraits::abstractType
{
  using abstractComponentType [[maybe_unused]] =
      typename TTakeContentTraits::componentTraits::abstractType;

  static_assert(
      isAnyComponent(meta::type_c<abstractComponentType>),
      "AnyAbstractTakeContentComponent requires AnyComponent.");


  using takeRefType [[maybe_unused]] =
      typename TTakeContentTraits::takeTraits::takeRefType;

  static_assert(
      isAnyTakeRef(meta::type_c<takeRefType>),
      "AnyAbstractTakeContentComponent requires AnyTakeRef.");

  using takeConstRefType [[maybe_unused]] =
      typename TTakeContentTraits::takeTraits::takeConstRefType;

  static_assert(
      isAnyTakeConstRef(meta::type_c<takeConstRefType>),
      "AnyAbstractTakeContentComponent requires AnyTakeConstRef.");


 public:
  [[maybe_unused]] inline AnyAbstractTakeContentComponent() = default;

  [[maybe_unused]] inline ~AnyAbstractTakeContentComponent() override = default;


  [[maybe_unused, nodiscard]] inline virtual const takeConstRefType&
  getTakeRef() const noexcept = 0;

  [[maybe_unused, nodiscard]] inline virtual takeRefType&
  getTakeRef() noexcept = 0;
};

template<typename TTakeContentTraits, typename THeldTakeTraits>
class [[maybe_unused]] AnyTakeContentComponentBase :
    public virtual TTakeContentTraits::abstractType,
    public TTakeContentTraits::componentTraits::baseType
{
  using abstractType [[maybe_unused]] =
      typename TTakeContentTraits::abstractType;


  using componentBaseType [[maybe_unused]] =
      typename TTakeContentTraits::componentTraits::baseType;

  static_assert(
      isAnyComponentBase<typename TTakeContentTraits::componentTraits>(
          meta::type_c<componentBaseType>),
      "AnyTakeContentComponentBase requires AnyComponentBase.");


  using contextType [[maybe_unused]] =
      typename TTakeContentTraits::
          componentTraits::
              contextualTraits::
                  contextType;

  static_assert(
      isAnyContext(meta::type_c<contextType>),
      "AnyTakeContentComponentBase requires AnyContext.");

  using stateType [[maybe_unused]] =
      typename TTakeContentTraits::
          componentTraits::
              statefulTraits::
                  stateType;

  static_assert(
      isAnyState(meta::type_c<stateType>),
      "AnyTakeContentComponentBase requires AnyState.");


  using takeRefType [[maybe_unused]] =
      typename TTakeContentTraits::takeTraits::takeRefType;

  static_assert(
      isAnyTakeRef(meta::type_c<takeRefType>),
      "AnyTakeContentComponentBase requires AnyTakeRef.");

  using takeConstRefType [[maybe_unused]] =
      typename TTakeContentTraits::takeTraits::takeConstRefType;

  static_assert(
      isAnyTakeConstRef(meta::type_c<takeConstRefType>),
      "AnyTakeContentComponentBase requires AnyTakeConstRef.");


  using heldTakeRefType [[maybe_unused]] =
      typename THeldTakeTraits::takeRefType;

  static_assert(
      meta::and_(
          isAnyTakeRef(meta::type_c<heldTakeRefType>),
          meta::traits::is_convertible(
              meta::type_c<heldTakeRefType&&>,
              meta::type_c<takeRefType&&>)),
      "AnyTakeContentComponentBase requires AnyTakeRef for "
      "held take ref type that is convertible to the takeRefType.");

  using heldTakeConstRefType [[maybe_unused]] =
      typename THeldTakeTraits::takeConstRefType;

  static_assert(
      meta::and_(
          isAnyTakeConstRef(meta::type_c<heldTakeConstRefType>),
          meta::traits::is_convertible(
              meta::type_c<heldTakeConstRefType&&>,
              meta::type_c<takeConstRefType&&>)),
      "AnyTakeContentComponentBase requires AnyTakeConstRef for "
      "held take const ref type that is convertible to the takeConstRefType.");


 public:
  [[maybe_unused]] explicit AnyTakeContentComponentBase(
      contextType&    context,
      stateType       state,
      heldTakeRefType take)
      : componentBaseType(
            context,
            move(state)),
        take(move(take))
  {
    BLOOPER_ASSERT(this->take.isValid());
  }


  [[maybe_unused, nodiscard]] inline const takeConstRefType&
  getTakeRef() const noexcept final
  {
    return this->getHeldTakeRef();
  }

  [[maybe_unused, nodiscard]] inline takeRefType&
  getTakeRef() noexcept final
  {
    return this->getHeldTakeRef();
  }


 protected:
  [[maybe_unused, nodiscard]] inline const heldTakeConstRefType&
  getHeldTakeRef() const noexcept
  {
    return this->take;
  }

  [[maybe_unused, nodiscard]] inline heldTakeRefType&
  getHeldTakeRef() noexcept
  {
    return this->take;
  }


 private:
  [[maybe_unused]] heldTakeRefType take;
};


using AbstractTakeTraits [[maybe_unused]] =
    AnyTakeTraits<AbstractTakeRef>;

using AbstractTakeRef [[maybe_unused]] =
    AbstractTakeTraits::takeRefType;

using AbstractTakeConstRef [[maybe_unused]] =
    AbstractTakeTraits::takeConstRefType;

BLOOPER_STATIC_ASSERT(
    isAnyTakeTraits(meta::type_c<AbstractTakeTraits>),
    "TakeTraits must satisfy AnyTakeTraits.");

BLOOPER_STATIC_ASSERT(
    isAnyTakeRef(meta::type_c<AbstractTakeRef>),
    "TakeRef must satisfy AnyTakeRef.");


using TakeTraits [[maybe_unused]] =
    AnyTakeTraits<TakeRef>;

using TakeRef [[maybe_unused]] =
    TakeTraits::takeRefType;

using TakeConstRef [[maybe_unused]] =
    TakeTraits::takeConstRefType;

BLOOPER_STATIC_ASSERT(
    isAnyTakeTraits(meta::type_c<TakeTraits>),
    "TakeTraits must satisfy AnyTakeTraits.");

BLOOPER_STATIC_ASSERT(
    isAnyTakeRef(meta::type_c<TakeRef>),
    "TakeRef must satisfy AnyTakeRef.");


using AbstractTakeContentTraits [[maybe_unused]] =
    AnyTakeContentTraits<
        AbstractTakeTraits,
        ComponentTraits>;

using AbstractTakeContentComponent [[maybe_unused]] =
    AbstractTakeContentTraits::abstractType;

using TakeContentTraits [[maybe_unused]] =
    AnyHeldTakeContentTraits<
        AbstractTakeContentTraits,
        TakeTraits>;

using TakeContentComponentBase [[maybe_unused]] =
    TakeContentTraits::baseType;

BLOOPER_STATIC_ASSERT(
    isAnyTakeContentTraits(
        meta::type_c<TakeContentTraits::contentTraits>),
    "TakeContentTraits must satisfy AnyTakeContentTraits.");

BLOOPER_STATIC_ASSERT(
    isAnyTakeContentComponent(
        meta::type_c<AbstractTakeContentComponent>),
    "AbstractTakeContentComponent must satisfy AnyTakeContentComponent.");

BLOOPER_STATIC_ASSERT(
    isAnyTakeContentComponentBase<
        TakeContentTraits::contentTraits,
        TakeTraits>(
        meta::type_c<TakeContentComponentBase>),
    "TakeContentComponentBase must satisfy AnyTakeContentComponentBase.");


using WaveAudioTakeTraits [[maybe_unused]] =
    AnyTakeTraits<WaveAudioTakeRef>;

using WaveAudioTakeRef [[maybe_unused]] =
    WaveAudioTakeTraits::takeRefType;

using WaveAudioTakeConstRef [[maybe_unused]] =
    WaveAudioTakeTraits::takeConstRefType;

BLOOPER_STATIC_ASSERT(
    isAnyTakeTraits(meta::type_c<WaveAudioTakeTraits>),
    "WaveAudioTakeTraits must satisfy AnyTakeTraits.");

BLOOPER_STATIC_ASSERT(
    isAnyTakeRef(meta::type_c<WaveAudioTakeRef>),
    "WaveAudioTakeRef must satisfy AnyTakeRef.");

BLOOPER_STATIC_ASSERT(
    isAnyTakeConstRef(meta::type_c<WaveAudioTakeConstRef>),
    "WaveAudioTakeConstRef must satisfy AnyTakeConstRef.");

using WaveAudioTakeContentTraits [[maybe_unused]] =
    AnyTakeContentTraits<
        WaveAudioTakeTraits,
        CoreComponentTraits>;

using AbstractWaveAudioTakeContentComponent [[maybe_unused]] =
    WaveAudioTakeContentTraits::abstractType;

using HeldWaveAudioTakeContentTraits [[maybe_unused]] =
    AnyHeldTakeContentTraits<
        WaveAudioTakeContentTraits,
        WaveAudioTakeTraits>;

using WaveAudioTakeContentComponentBase [[maybe_unused]] =
    HeldWaveAudioTakeContentTraits::baseType;

BLOOPER_STATIC_ASSERT(
    isAnyTakeContentTraits(
        meta::type_c<WaveAudioTakeContentTraits>),
    "WaveAudioTakeContentTraits must satisfy AnyTakeContentTraits.");

BLOOPER_STATIC_ASSERT(
    isAnyTakeContentComponent(
        meta::type_c<AbstractWaveAudioTakeContentComponent>),
    "AbstractWaveAudioTakeContentComponent must satisfy AnyTakeContentComponent.");


using MidiTakeTraits [[maybe_unused]] =
    AnyTakeTraits<MidiTakeRef>;

using MidiTakeRef [[maybe_unused]] =
    MidiTakeTraits::takeRefType;

using MidiTakeConstRef [[maybe_unused]] =
    MidiTakeTraits::takeConstRefType;

BLOOPER_STATIC_ASSERT(
    isAnyTakeTraits(meta::type_c<MidiTakeTraits>),
    "MidiTakeTraits must satisfy AnyTakeTraits.");

BLOOPER_STATIC_ASSERT(
    isAnyTakeRef(meta::type_c<MidiTakeRef>),
    "MidiTakeRef must satisfy AnyTakeRef.");

BLOOPER_STATIC_ASSERT(
    isAnyTakeConstRef(meta::type_c<MidiTakeConstRef>),
    "MidiTakeConstRef must satisfy AnyTakeConstRef.");

using MidiTakeContentTraits [[maybe_unused]] =
    AnyTakeContentTraits<
        MidiTakeTraits,
        CoreComponentTraits>;

using AbstractMidiTakeContentComponent [[maybe_unused]] =
    MidiTakeContentTraits::abstractType;

using HeldMidiTakeContentTraits [[maybe_unused]] =
    AnyHeldTakeContentTraits<
        MidiTakeContentTraits,
        MidiTakeTraits>;

using MidiTakeContentComponentBase [[maybe_unused]] =
    HeldMidiTakeContentTraits::baseType;

BLOOPER_STATIC_ASSERT(
    isAnyTakeContentTraits(
        meta::type_c<MidiTakeContentTraits>),
    "MidiTakeContentTraits must satisfy AnyTakeContentTraits.");

BLOOPER_STATIC_ASSERT(
    isAnyTakeContentComponent(
        meta::type_c<AbstractMidiTakeContentComponent>),
    "AbstractMidiTakeContentComponent must satisfy AnyTakeContentComponent.");

BLOOPER_STATIC_ASSERT(
    isAnyTakeContentComponentBase<
        MidiTakeContentTraits,
        MidiTakeTraits>(
        meta::type_c<MidiTakeContentComponentBase>),
    "MidiTakeContentComponentBase must satisfy AnyTakeContentComponentBase.");


// Explicit instantiation

extern template class AnyAbstractTakeContentComponent<
    AbstractTakeContentTraits>;


extern template class AnyTakeContentComponentBase<
    AbstractTakeContentTraits,
    WaveAudioTakeTraits>;

extern template class AnyTakeContentComponentBase<
    AbstractTakeContentTraits,
    MidiTakeTraits>;

BLOOPER_NAMESPACE_END

#endif // BLOOPER_ABSTRACT_TAKES_HPP
