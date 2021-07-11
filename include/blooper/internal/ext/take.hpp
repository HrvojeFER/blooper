#ifndef BLOOPER_TAKE_HPP
#define BLOOPER_TAKE_HPP

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/traits.hpp>
#include <blooper/internal/abstract/predicates.hpp>
#include <blooper/internal/abstract/visitors.hpp>

BLOOPER_EXT_NAMESPACE_BEGIN

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


  inline virtual juce::ReferenceCountedObjectPtr<te::Clip>
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

  TakeRefBase(TClip* clip, int index) noexcept
      : clip(clip),
        index(index)
  {
  }

  [[maybe_unused]] TClip* clip;

  [[maybe_unused]] int index;


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
};

class [[maybe_unused]] WaveAudioTakeRef final :
    public virtual AbstractTakeRef,
    public TakeRefBase<te::WaveAudioClip>
{
 public:
  WaveAudioTakeRef() noexcept = default;

  WaveAudioTakeRef(
      te::WaveAudioClip* clip,
      int                index,
      te::ProjectItemID  audioFileId) noexcept
      : TakeRefBase(clip, index),
        audioFileId(move(audioFileId))
  {
  }

  te::ProjectItemID audioFileId;


  [[nodiscard]] inline bool isValid() const noexcept final
  {
    return TakeRefBase::isValid() && this->audioFileId.isValid();
  }

  [[nodiscard]] inline bool isInvalid() const noexcept final
  {
    return TakeRefBase::isInvalid() && this->audioFileId.isInvalid();
  }


  [[nodiscard]] inline bool isComp() const noexcept final
  {
    return this->clip->getCompManager().isTakeComp(this->index);
  }


  inline juce::ReferenceCountedObjectPtr<te::Clip>
  unpackInto(te::ClipTrack& track) const final
  {
    if (this->isComp()) return nullptr;

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

  te::MidiList* midi;


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


  inline juce::ReferenceCountedObjectPtr<te::Clip>
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
      clip->getSequence()
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

BLOOPER_EXT_NAMESPACE_END

#endif // BLOOPER_TAKE_HPP
