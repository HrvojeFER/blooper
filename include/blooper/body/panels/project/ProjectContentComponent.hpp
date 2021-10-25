#ifndef BLOOPER_PROJECT_CONTENT_COMPONENT_HPP
#define BLOOPER_PROJECT_CONTENT_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

#include <blooper/internal/utils/FlaggedAsyncUpdater.hpp>

BLOOPER_NAMESPACE_BEGIN

class ProjectContentComponent :
    public ComponentBase,

    private util::FlaggedAsyncUpdater,
    private juce::ApplicationCommandTarget
{
 public:
  BLOOPER_STATE_ID(ProjectContentComponent);


  struct Options
  {
  } options;

  explicit ProjectContentComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});

  ~ProjectContentComponent() override;


 private:
  class Pimpl;
  friend Pimpl;
  std::unique_ptr<Pimpl> tabs;

  juce::OwnedArray<class EditComponent> editComponents;


  JuceFlag editUpdate;

  void updateEdits();


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


  // FlaggedAsyncUpdater

 private:
  void handleAsyncUpdate() override;


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
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectContentComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PROJECT_CONTENT_COMPONENT_HPP
