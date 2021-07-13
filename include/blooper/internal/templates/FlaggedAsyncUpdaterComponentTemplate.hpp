#ifndef BLOOPER___NAME___COMPONENT_HPP
#define BLOOPER___NAME___COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

#include <blooper/internal/utils/FlaggedAsyncUpdater.hpp>

BLOOPER_NAMESPACE_BEGIN

struct __name__ComponentOptions
{
};

class __name__Component :
    public ComponentBase,

    private util::FlaggedAsyncUpdater
{
 public:
  BLOOPER_STATE_ID(__name__Component);


  explicit __name__Component(
      AbstractContext&         context,
      State                    state,
      __name__ComponentOptions options = {});

  ~__name__Component() override;

  __name__ComponentOptions options;


  // Component

 public:
  void resized() override;


  // ValueTreeListener

 private:
  void valueTreePropertyChanged(
      juce::ValueTree&        tree,
      const juce::Identifier& id) override;

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


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(__name__Component)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER___NAME___COMPONENT_HPP
