#ifndef BLOOPER_BROWSER_PANEL_COMPONENT_HPP
#define BLOOPER_BROWSER_PANEL_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

class BrowserPanelComponent :
    public ComponentBase,

    private juce::FileBrowserListener
{
 public:
  BLOOPER_STATE_ID(BrowserPanelComponent);


  struct Options
  {
    std::function<void(const juce::File&)> onFileSelected;
  } options;

  explicit BrowserPanelComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});

  ~BrowserPanelComponent() override;


 private:
  std::unique_ptr<juce::FileBrowserComponent> browser;

  std::unique_ptr<class BrowserPreviewComponent> preview;


  // Component

 public:
  void paint(JuceGraphics&) override;

  void resized() override;


  // FileBrowserListener

 private:
  void selectionChanged() override;

  void fileClicked(
      const juce::File&       file,
      const juce::MouseEvent& e) override;

  void fileDoubleClicked(const juce::File& file) override;

  void browserRootChanged(const juce::File& newRoot) override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BrowserPanelComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_BROWSER_PANEL_COMPONENT_HPP
