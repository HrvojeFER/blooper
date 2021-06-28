#ifndef BLOOPER_TRACK_PLUGIN_LIST_COMPONENT_HPP
#define BLOOPER_TRACK_PLUGIN_LIST_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

#include <blooper/internal/utils/FlaggedAsyncUpdater.hpp>

BLOOPER_NAMESPACE_BEGIN

class TrackPluginsComponent :
    public ComponentBase,

    private juce::ListBoxModel,
    private juce::ApplicationCommandTarget
{
 public:
  BLOOPER_STATE_ID(TrackPluginsComponent);


  struct Options
  {
  } options;

  explicit TrackPluginsComponent(
      AbstractContext&         context,
      State                    state,
      JuceRef<class EditTrack> track,
      Options                  options = {});

  ~TrackPluginsComponent() override;


 private:
  JuceRef<class EditTrack> track;


  std::unique_ptr<juce::ListBox> list;


  [[nodiscard]] bool isValidRow(int row);


  // Component

 public:
  void resized() override;


  // ValueTreeListener

 private:
  void valueTreeChildAdded(
      juce::ValueTree& tree,
      juce::ValueTree& child) override;

  void valueTreeChildRemoved(
      juce::ValueTree& tree,
      juce::ValueTree& child,
      int              index) override;

  void valueTreeChildOrderChanged(
      juce::ValueTree& tree,
      int              childAIndex,
      int              childBIndex) override;


  // ListBoxModel

 private:
  int getNumRows() override;

  void paintListBoxItem(
      int             row,
      juce::Graphics& g,
      int             width,
      int             height,
      bool            rowIsSelected) override;

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

  void getAllCommands(juce::Array<juce::CommandID>& commands) override;

  void getCommandInfo(
      juce::CommandID               commandID,
      juce::ApplicationCommandInfo& result) override;

  bool perform(const InvocationInfo& info) override;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackPluginsComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_TRACK_PLUGIN_LIST_COMPONENT_HPP
