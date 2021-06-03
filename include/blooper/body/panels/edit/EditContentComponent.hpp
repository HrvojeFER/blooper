#ifndef BLOOPER_EDIT_CONTENT_COMPONENT_HPP
#define BLOOPER_EDIT_CONTENT_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class EditContentComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(EditContentComponent);

  [[maybe_unused]] inline constexpr static auto defaultTrackSize = 20;


  struct Options
  {
  } options;

  explicit EditContentComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});


  void resized() override;


 private:
  juce::OwnedArray<class EditTrackComponent> tracks;


  [[maybe_unused]] bool isValid(int index);

  [[maybe_unused]] void addTrack(int at = -1);

  [[maybe_unused]] void removeTrack(int at = -1);

  [[maybe_unused]] void resizeToFitTracks();


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EditContentComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_EDIT_CONTENT_COMPONENT_HPP
