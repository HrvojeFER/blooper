#ifndef BLOOPER_PLUGIN_EDITOR_COMPONENT_HPP
#define BLOOPER_PLUGIN_EDITOR_COMPONENT_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class PluginEditorComponent : public CoreComponentBase
{
 public:
  BLOOPER_STATE_ID(PluginEditorComponent);

  using contentType = class AbstractPluginContentComponent;


  struct Options
  {
  } options;

  explicit PluginEditorComponent(
      AbstractCoreContext& context,
      State                state,
      JucePluginRef        plugin,
      Options              options = {});


  [[maybe_unused, nodiscard]] inline const JucePlugin&
  getPlugin() const noexcept;

  [[maybe_unused, nodiscard]] inline JucePluginConstRef
  getPluginRef() const noexcept;


  [[maybe_unused, nodiscard]] inline const contentType&
  getContent() const noexcept;

  [[maybe_unused, nodiscard]] inline contentType&
  getContent() noexcept;


  void resized() override;


 private:
  [[maybe_unused, nodiscard]] inline JucePlugin&
  getPlugin() noexcept;

  [[maybe_unused, nodiscard]] inline JucePluginRef
  getPluginRef() noexcept;


  JucePluginRef plugin;

  std::unique_ptr<contentType> content;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditorComponent)
};


[[maybe_unused]] const JucePlugin&
PluginEditorComponent::getPlugin() const noexcept
{
  return *plugin;
}

[[maybe_unused]] JucePlugin&
PluginEditorComponent::getPlugin() noexcept
{
  return *plugin;
}

[[maybe_unused]] JucePluginConstRef
PluginEditorComponent::getPluginRef() const noexcept
{
  return plugin;
}

[[maybe_unused]] JucePluginRef
PluginEditorComponent::getPluginRef() noexcept
{
  return plugin;
}

[[maybe_unused]] const PluginEditorComponent::contentType&
PluginEditorComponent::getContent() const noexcept
{
  return *content;
}

[[maybe_unused]] PluginEditorComponent::contentType&
PluginEditorComponent::getContent() noexcept
{
  return *content;
}

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PLUGIN_EDITOR_COMPONENT_HPP
