#ifndef BLOOPER_TOOLBAR_COMPONENT_HPP
#define BLOOPER_TOOLBAR_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

class HeaderToolbarComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(HeaderToolbarComponent);

  BLOOPER_ID(toolbarStateId);


  struct Options
  {
  } options;

  explicit HeaderToolbarComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});

  ~HeaderToolbarComponent() override;


 private:
  class ItemFactory;
  friend ItemFactory;
  std::unique_ptr<ItemFactory> factory;

  std::unique_ptr<juce::Toolbar> toolbar;

  std::unique_ptr<juce::DrawableButton> toolbarCustomizationButton;


  // Component

 public:
  void resized() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HeaderToolbarComponent)
};

BLOOPER_NAMESPACE_END

#endif //BLOOPER_TOOLBAR_COMPONENT_HPP
