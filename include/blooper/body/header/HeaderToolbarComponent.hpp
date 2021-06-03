#ifndef BLOOPER_TOOLBAR_COMPONENT_HPP
#define BLOOPER_TOOLBAR_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

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

  ~HeaderToolbarComponent();


  void resized() override;


 private:
  std::unique_ptr<juce::Toolbar> toolbar;

  class ItemFactory;
  std::unique_ptr<ItemFactory> factory;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HeaderToolbarComponent)
};

BLOOPER_NAMESPACE_END

#endif //BLOOPER_TOOLBAR_COMPONENT_HPP
