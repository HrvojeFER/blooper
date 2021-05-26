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


  [[maybe_unused, nodiscard]] inline const JuceConstrainer&
  getConstrainer() const noexcept;

  [[maybe_unused, nodiscard]] inline JuceConstrainer&
  getConstrainer() noexcept;


  void resized() override;


 private:
  [[maybe_unused, nodiscard]] inline JucePlugin&
  getPlugin() noexcept;

  [[maybe_unused, nodiscard]] inline JucePluginRef
  getPluginRef() noexcept;


  JucePluginRef plugin;

  std::unique_ptr<contentType> content;

  std::unique_ptr<JuceConstrainer> constrainer;


  [[maybe_unused, nodiscard]] inline std::unique_ptr<JuceConstrainer>
  createConstrainer() const noexcept;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditorComponent)
};


[[maybe_unused]] const JucePlugin&
PluginEditorComponent::getPlugin() const noexcept
{
  return *this->plugin;
}

[[maybe_unused]] JucePlugin&
PluginEditorComponent::getPlugin() noexcept
{
  return *this->plugin;
}

[[maybe_unused]] JucePluginConstRef
PluginEditorComponent::getPluginRef() const noexcept
{
  return this->plugin;
}

[[maybe_unused]] JucePluginRef
PluginEditorComponent::getPluginRef() noexcept
{
  return this->plugin;
}

[[maybe_unused]] const PluginEditorComponent::contentType&
PluginEditorComponent::getContent() const noexcept
{
  return *this->content;
}

[[maybe_unused]] PluginEditorComponent::contentType&
PluginEditorComponent::getContent() noexcept
{
  return *this->content;
}

[[maybe_unused]] const JuceConstrainer&
PluginEditorComponent::getConstrainer() const noexcept
{
  return *this->constrainer;
}

[[maybe_unused]] JuceConstrainer&
PluginEditorComponent::getConstrainer() noexcept
{
  return *this->constrainer;
}


[[maybe_unused]] std::unique_ptr<JuceConstrainer>
PluginEditorComponent::createConstrainer() const noexcept
{
  auto contentConstrainer = this->getContent().getConstrainer();

  if (contentConstrainer)
  {
    auto componentConstrainer = std::make_unique<JuceConstrainer>();

    util::copyConstrainer(
        *contentConstrainer,
        *componentConstrainer);

    return componentConstrainer;
  }
  else
  {
    return {};
  }
}


BLOOPER_NAMESPACE_END

#endif // BLOOPER_PLUGIN_EDITOR_COMPONENT_HPP
