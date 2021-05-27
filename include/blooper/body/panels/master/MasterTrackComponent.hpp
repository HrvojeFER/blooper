#ifndef BLOOPER_MASTER_TRACK_COMPONENT_HPP
#define BLOOPER_MASTER_TRACK_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class MasterTrackComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(MasterTrackComponent);


  struct Options
  {
  } options;

  explicit MasterTrackComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});


  void resized() override;


 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MasterTrackComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_MASTER_TRACK_COMPONENT_HPP
