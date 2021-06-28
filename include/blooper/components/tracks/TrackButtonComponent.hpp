#ifndef BLOOPER_TRACK_BUTTON_COMPONENT_HPP
#define BLOOPER_TRACK_BUTTON_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

#include <blooper/internal/utils/FlaggedAsyncUpdater.hpp>

BLOOPER_NAMESPACE_BEGIN

struct TrackButtonOptions
{
};

class TrackButtonComponent final :
    public ComponentBase,

    private util::FlaggedAsyncUpdater
{
 public:
  BLOOPER_STATE_ID(TrackButtonComponent);


  explicit TrackButtonComponent(
      AbstractContext&         context,
      State                    state,
      JuceRef<class EditTrack> track,
      TrackButtonOptions       options = {});

  ~TrackButtonComponent() override;

  TrackButtonOptions options;


 private:
  JuceRef<class EditTrack> track;

  class Pimpl;
  friend Pimpl;
  std::unique_ptr<Pimpl> pimpl;


  JuceFlag imageUpdate;

  void updateImages();


  // Component

 public:
  void resized() override;


  // ValueTreeListener

 private:
  void valueTreePropertyChanged(
      juce::ValueTree&        tree,
      const juce::Identifier& id) override;


  // FlaggedAsyncUpdater

 private:
  void handleAsyncUpdate() override;
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_TRACK_BUTTON_COMPONENT_HPP
