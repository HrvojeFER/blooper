#ifndef BLOOPER_EDIT_COMPONENT_HPP
#define BLOOPER_EDIT_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

struct EditOptions
{
};

class EditComponent : public ComponentBase
{
 public:
  BLOOPER_STATE_ID(EditComponent);


  explicit EditComponent(
      AbstractContext& context,
      State            state,
      JuceEditRef      edit,
      EditOptions      options = {});

  ~EditComponent() override;

  EditOptions options;


  [[maybe_unused, nodiscard]] inline JuceEditConstRef
  getEdit() const noexcept;

  [[maybe_unused, nodiscard]] inline JuceEditRef
  getEdit() noexcept;


 private:
  BLOOPER_ID(editScrollStartId);
  JuceCached<double> editScrollStart;

  BLOOPER_ID(editScrollEndId);
  JuceCached<double> editScrollEnd;


  JuceEditRef edit;

  std::unique_ptr<class EditContentComponent> content;

  std::unique_ptr<juce::Viewport> viewport;


  // Component

 public:
  void paint(JuceGraphics&) override;

  void resized() override;


  // Declarations

 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EditComponent)
};


JuceEditRef EditComponent::getEdit() noexcept
{
  return this->edit;
}

JuceEditConstRef EditComponent::getEdit() const noexcept
{
  return this->edit;
}

BLOOPER_NAMESPACE_END

#endif // BLOOPER_EDIT_COMPONENT_HPP
