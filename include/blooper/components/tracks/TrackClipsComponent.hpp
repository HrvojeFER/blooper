#ifndef BLOOPER_TRACK_CLIPS_COMPONENT_HPP
#define BLOOPER_TRACK_CLIPS_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

struct TrackClipsOptions
{
};

class TrackClipsComponent :
    public ComponentBase,

    private juce::ListBoxModel,
    private juce::ApplicationCommandTarget
{
 public:
  BLOOPER_STATE_ID(TrackClipsComponent);


  explicit TrackClipsComponent(
      AbstractContext&         context,
      State                    state,
      JuceRef<class EditTrack> track,
      TrackClipsOptions        options = {});

  ~TrackClipsComponent() override;

  TrackClipsOptions options;


 private:
  JuceRef<class EditTrack> track;

  std::unique_ptr<juce::ListBox> list;


  [[maybe_unused, nodiscard]] bool isValidRow(int row) const noexcept;


  // Component

 public:
  [[maybe_unused]] void resized() override;


  // ListBoxModel

 public:
  int getNumRows() override;

  void paintListBoxItem(
      int             rowNumber,
      juce::Graphics& g,
      int             width,
      int             height,
      bool            isSelected) override;

  void listBoxItemClicked(
      int                     row,
      const juce::MouseEvent& event) override;

  void listBoxItemDoubleClicked(
      int                     row,
      const juce::MouseEvent& event) override;

  juce::String getTooltipForRow(int row) override;


  // ApplicationCommandTarget

 private:
  ApplicationCommandTarget* getNextCommandTarget() override;

  void getAllCommands(
      juce::Array<juce::CommandID>& commands) override;

  void getCommandInfo(
      juce::CommandID               commandID,
      juce::ApplicationCommandInfo& result) override;

  bool perform(const InvocationInfo& info) override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackClipsComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_TRACK_CLIPS_COMPONENT_HPP
