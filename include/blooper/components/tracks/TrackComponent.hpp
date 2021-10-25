#ifndef BLOOPER_EDIT_TRACK_COMPONENT_HPP
#define BLOOPER_EDIT_TRACK_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

class TrackComponent :
    public ComponentBase,

    private juce::Label::Listener,
    private juce::ComboBox::Listener,

    private juce::ChangeListener
{
 public:
  BLOOPER_STATE_ID(TrackComponent);


  struct Options
  {
  } options;

  explicit TrackComponent(
      AbstractContext& context,
      State            state,
      JuceTrackRef     track,
      Options          options = {});

  ~TrackComponent() override;


 private:
  JuceTrackRef track;

  std::unique_ptr<juce::Label> name;

  std::unique_ptr<class TrackPlayheadComponent> playhead;
  std::unique_ptr<class TrackButtonComponent>   button;

  std::unique_ptr<juce::DrawableButton>
      muteButton,
      soloButton,
      armButton;

  std::unique_ptr<juce::ComboBox> modeDropdown;
  std::unique_ptr<juce::ComboBox> intervalDropdown;

  std::unique_ptr<class TrackPluginsComponent> pluginList;

  JuceState                                  appearanceSettings;
  JuceCached<double>                         clipsSize;
  std::unique_ptr<class TrackClipsComponent> clips;


  // Component

 public:
  void paint(JuceGraphics& g) override;

  void resized() override;

  void mouseDown(const juce::MouseEvent& event) override;


  // ValueTreeListener

 private:
  void valueTreePropertyChanged(
      juce::ValueTree&        tree,
      const juce::Identifier& id) override;


  // LabelListener

 private:
  void labelTextChanged(
      juce::Label* labelThatHasChanged) override;


  // ComboBoxListener

 private:
  void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;


  // ChangeListener

 private:
  void changeListenerCallback(juce::ChangeBroadcaster* broadcaster) override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackComponent)
  JUCE_DECLARE_WEAK_REFERENCEABLE(TrackComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_EDIT_TRACK_COMPONENT_HPP
