#ifndef BLOOPER_HEADER_COMPONENT_HPP
#define BLOOPER_HEADER_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class HeaderComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(HeaderComponent);


  struct Options
  {
  } options;

  explicit HeaderComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});


  void resized() override;


 private:
  std::unique_ptr<class HeaderMenuBarComponent> menuBar;
  std::unique_ptr<class HeaderToolbarComponent> toolbar;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HeaderComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_HEADER_COMPONENT_HPP
