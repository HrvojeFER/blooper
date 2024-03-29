#ifndef BLOOPER_EDIT_TRACK_HPP
#define BLOOPER_EDIT_TRACK_HPP

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/abstract.hpp>
#include <blooper/internal/ext/ext.hpp>

BLOOPER_NAMESPACE_BEGIN

// indices so it works with dropdowns

enum class TrackMode : int
{
  sync [[maybe_unused]] = 1,
  oneShot [[maybe_unused]] = 2,
  free [[maybe_unused]] = 3,
};

enum class TrackPlayback : int
{
  playing [[maybe_unused]] = 1,
  paused [[maybe_unused]] = 2,
  recording [[maybe_unused]] = 3
};

class [[maybe_unused]] EditTrack :
    public ContextualBase,
    public StatefulBase,
    public juce::ReferenceCountedObject,
    public te::Selectable
{
 public:
  using Id =
      std::decay_t<
          decltype(std::declval<JuceProjectItem>()
                       .getID()
                       .getRawID())>;

  inline constexpr static Id invalidId = -1;


  explicit EditTrack(
      AbstractContext&   context,
      State              state,
      JuceProjectItemRef item);

  ~EditTrack() override;


  [[maybe_unused]] inline const JuceEdit&
  getEdit() const noexcept;

  [[maybe_unused]] inline JuceEdit&
  getEdit() noexcept;


  [[maybe_unused]] inline const JuceAudioTrack&
  getAudio() const noexcept;

  [[maybe_unused]] inline JuceAudioTrack&
  getAudio() noexcept;


  [[nodiscard]] inline Id getId() const noexcept;


  JuceCached<TrackMode>     mode;
  JuceCached<Interval>      interval;
  JuceCached<TrackPlayback> playback;

  JuceCached<bool>
      muted,
      armed;


  [[maybe_unused, nodiscard]] bool isSoloed() const noexcept;

  [[maybe_unused]] void setSoloed(bool) noexcept;

  [[maybe_unused]] void toggleSoloed() noexcept;


 private:
  JuceCached<Id> id;

  JuceProjectItemRef item;
  JuceEditRef        edit;
  JuceTransportRef   transport;
  JuceAudioTrackRef  audio;

  Token syncToken;
  Token recordToken;


  void synchronize();


  // ValueTreeListener

 private:
  void valueTreePropertyChanged(
      juce::ValueTree&        tree,
      const juce::Identifier& _id) override;


  // Selectable

 public:
  juce::String getSelectableDescription() override;


  // EditManager

 private:
  friend class EditManager;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EditTrack);
  JUCE_DECLARE_WEAK_REFERENCEABLE(EditTrack);
};

using EditTrackRef [[maybe_unused]] = JuceRef<EditTrack>;
using EditTrackConstRef [[maybe_unused]] = JuceConstRef<EditTrack>;


[[nodiscard, maybe_unused]] const JuceAudioTrack&
EditTrack::getAudio() const noexcept
{
  return *this->audio;
}

[[nodiscard, maybe_unused]] JuceAudioTrack&
EditTrack::getAudio() noexcept
{
  return *this->audio;
}


[[nodiscard, maybe_unused]] const JuceEdit&
EditTrack::getEdit() const noexcept
{
  return *this->edit;
}

[[nodiscard, maybe_unused]] JuceEdit&
EditTrack::getEdit() noexcept
{
  return *this->edit;
}


[[nodiscard]] EditTrack::Id EditTrack::getId() const noexcept
{
  return id;
}


[[maybe_unused, nodiscard]] inline EditTrack::Id
getId(const EditTrack& track) noexcept
{
  return track.getId();
}

[[maybe_unused, nodiscard]] inline EditTrack::Id
getEditTrackId(const JuceProjectItemId& id) noexcept
{
  return id.getRawID();
}

[[maybe_unused, nodiscard]] inline EditTrack::Id
getEditTrackId(const JuceProjectItem& item) noexcept
{
  return getEditTrackId(item.getID());
}

[[maybe_unused, nodiscard]] inline EditTrack::Id
getEditTrackId(const JuceEdit& edit) noexcept
{
  return getEditTrackId(edit.getProjectItemID());
}

[[maybe_unused, nodiscard]] inline EditTrack::Id
getEditTrackId(const JuceTransport& transport) noexcept
{
  return getEditTrackId(transport.edit);
}

[[maybe_unused, nodiscard]] inline EditTrack::Id
getEditTrackId(const JuceTrack& track) noexcept
{
  return getEditTrackId(track.edit);
}


[[maybe_unused, nodiscard]] inline bool
operator==(const EditTrack& a, const EditTrack& b) noexcept
{
  return getId(a) == getId(b);
}

[[maybe_unused, nodiscard]] inline bool
operator==(const EditTrack& a, EditTrack::Id b) noexcept
{
  return getId(a) == b;
}

[[maybe_unused, nodiscard]] inline bool
operator==(EditTrack::Id a, const EditTrack& b) noexcept
{
  return a == getId(b);
}

[[maybe_unused, nodiscard]] inline bool
operator!=(const EditTrack& a, const EditTrack& b) noexcept
{
  return getId(a) != getId(b);
}

[[maybe_unused, nodiscard]] inline bool
operator!=(const EditTrack& a, EditTrack::Id b) noexcept
{
  return getId(a) != b;
}

[[maybe_unused, nodiscard]] inline bool
operator!=(EditTrack::Id a, const EditTrack& b) noexcept
{
  return a != getId(b);
}

BLOOPER_NAMESPACE_END


namespace juce
{
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
struct VariantConverter<blooper::TrackPlayback>
{
  static blooper::TrackPlayback fromVar(const var& v)
  {
    return static_cast<blooper::TrackPlayback>(static_cast<int>(v));
  }
  static var toVar(const blooper::TrackPlayback& i)
  {
    return static_cast<int>(i);
  }
};
} // namespace juce

namespace std
{
template<>
struct hash<blooper::EditTrack>
{
  size_t operator()(const blooper::EditTrack& track)
  {
    constexpr hash<blooper::EditTrack::Id> idHash{};

    return idHash(blooper::getId(track));
  }
};
} // namespace std

#endif //BLOOPER_EDIT_TRACK_HPP
