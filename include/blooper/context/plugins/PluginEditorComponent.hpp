#ifndef BLOOPER_PLUGIN_EDITOR_COMPONENT_HPP
#define BLOOPER_PLUGIN_EDITOR_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class PluginEditorComponent final : public PluginContentComponentBase
{
 public:
  BLOOPER_STATE_ID(PluginEditorComponent);


  struct Options
  {
  } options;

  explicit PluginEditorComponent(
      AbstractCoreContext& context,
      State                state,
      JucePluginRef        plugin,
      Options              options = {});


  [[maybe_unused, nodiscard]] inline const JuceConstrainer*
  getConstrainer() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceConstrainer*
  getConstrainer() noexcept final;


  [[maybe_unused, nodiscard]] inline bool
  checkIsResizable() const noexcept final;


  [[maybe_unused]] inline void recreate() final;


  void resized() override;


 private:
  std::unique_ptr<AbstractPluginContentComponent> content;

  std::unique_ptr<JuceConstrainer> constrainer;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditorComponent)
};


[[maybe_unused]] const JuceConstrainer*
PluginEditorComponent::getConstrainer() const noexcept
{
  return this->constrainer.get();
}

[[maybe_unused]] JuceConstrainer*
PluginEditorComponent::getConstrainer() noexcept
{
  return this->constrainer.get();
}


[[maybe_unused]] bool
PluginEditorComponent::checkIsResizable() const noexcept
{
  return content->checkIsResizable();
}

[[maybe_unused]] void
PluginEditorComponent::recreate()
{
  content->recreate();
}

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PLUGIN_EDITOR_COMPONENT_HPP
