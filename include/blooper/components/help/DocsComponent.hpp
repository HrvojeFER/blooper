#ifndef BLOOPER_DOCS_COMPONENT_HPP
#define BLOOPER_DOCS_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

class DocsComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(DocsComponent);


  struct Options
  {
  } options;

  explicit DocsComponent(
      AbstractContext& context,
      State            state,
      Options          options = {});

  ~DocsComponent() override;


  // Component

 public:
  [[maybe_unused]] void resized() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DocsComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_DOCS_COMPONENT_HPP
