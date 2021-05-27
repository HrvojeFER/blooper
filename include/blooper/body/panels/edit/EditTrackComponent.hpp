#ifndef BLOOPER_EDIT_TRACK_COMPONENT_HPP
#define BLOOPER_EDIT_TRACK_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class EditTrackComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(EditTrackComponent);


  struct Options
  {
  } options;

  explicit EditTrackComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});


  void resized() override;


 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EditTrackComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_EDIT_TRACK_COMPONENT_HPP
