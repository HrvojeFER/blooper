#ifndef BLOOPER_BROWSER_PREVIEW_COMPONENT_HPP
#define BLOOPER_BROWSER_PREVIEW_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

class BrowserPreviewComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(BrowserPreviewComponent);


  struct Options
  {
  } options;

  explicit BrowserPreviewComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});

  ~BrowserPreviewComponent() override;


  // Component

 public:
  void resized() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BrowserPreviewComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_BROWSER_PREVIEW_COMPONENT_HPP
