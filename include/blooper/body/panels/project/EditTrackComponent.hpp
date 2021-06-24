#ifndef BLOOPER_EDIT_TRACK_COMPONENT_HPP
#define BLOOPER_EDIT_TRACK_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

class EditTrackComponent :
    public ComponentBase,

    private juce::Label::Listener,
    private juce::ComboBox::Listener
{
 public:
  BLOOPER_STATE_ID(EditTrackComponent);


  struct Options
  {
  } options;

  explicit EditTrackComponent(
      AbstractContext& context,
      State            state,
      EditTrackRef     track,
      Options          options = {});

  ~EditTrackComponent() override;


  [[maybe_unused, nodiscard]] inline const EditTrack&
  getTrack() const noexcept;

  [[maybe_unused, nodiscard]] inline EditTrack&
  getTrack() noexcept;


 private:
  EditTrackRef track;

  std::unique_ptr<juce::Label> name;

  class ButtonPlayhead;
  class Button;
  std::unique_ptr<Button> button;

  std::unique_ptr<juce::DrawableButton>
      muteButton,
      soloButton,
      armButton;

  std::unique_ptr<juce::ComboBox> modeDropdown;
  std::unique_ptr<juce::ComboBox> intervalDropdown;

  std::unique_ptr<class TrackPluginListComponent> pluginList;


  // Component

 public:
  void resized() override;

  void mouseDown(const juce::MouseEvent& event) override;


  // ValueTreeListener

 private:
  void
  valueTreePropertyChanged(
      juce::ValueTree&        tree,
      const juce::Identifier& id) override;


  // LabelListener

 private:
  void labelTextChanged(
      juce::Label* labelThatHasChanged) override;


  // ComboBoxListener

 private:
  void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EditTrackComponent)
  JUCE_DECLARE_WEAK_REFERENCEABLE(EditTrackComponent)
};


const EditTrack& EditTrackComponent::getTrack() const noexcept
{
  return *this->track;
}

EditTrack& EditTrackComponent::getTrack() noexcept
{
  return *this->track;
}

BLOOPER_NAMESPACE_END

#endif // BLOOPER_EDIT_TRACK_COMPONENT_HPP
