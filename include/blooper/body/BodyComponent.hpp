#ifndef BLOOPER_BODY_COMPONENT_HPP
#define BLOOPER_BODY_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class BodyComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(BodyComponent);


  struct Options
  {
  } options;

  explicit BodyComponent(
      AbstractContext& context,
      State            state,
      Options          options);


  void resized() override;


 private:
  std::unique_ptr<class PanelsComponent> panels;

  std::unique_ptr<class HeaderComponent> header;
  std::unique_ptr<class FooterComponent> footer;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BodyComponent);
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_BODY_COMPONENT_HPP
