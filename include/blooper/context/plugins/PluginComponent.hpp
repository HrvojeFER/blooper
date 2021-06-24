#ifndef BLOOPER_PLUGIN_COMPONENT_HPP
#define BLOOPER_PLUGIN_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

class PluginComponent : public CoreComponentBase
{
 public:
  BLOOPER_STATE_ID(PluginComponent);


  struct Options
  {
  } options;

  [[nodiscard]] explicit PluginComponent(
      AbstractCoreContext& context,
      State                state,
      JucePluginRef        plugin,
      Options              options = {});


  [[maybe_unused, nodiscard]] inline const JucePlugin&
  getPlugin() const noexcept;

  [[maybe_unused, nodiscard]] inline JucePluginConstRef
  getPluginRef() const noexcept;


  void resized() override;


 private:
  [[maybe_unused, nodiscard]] inline JucePlugin&
  getPlugin() noexcept;

  [[maybe_unused, nodiscard]] inline JucePluginRef
  getPluginRef() noexcept;


  JucePluginRef plugin;


  class Button : public juce::TextButton
  {
   public:
    explicit Button(PluginComponent* parent);

    void clicked(const juce::ModifierKeys& modifiers) override;


   private:
    PluginComponent* parent;

  } button;
};


[[maybe_unused]] const JucePlugin&
PluginComponent::getPlugin() const noexcept
{
  return *plugin;
}

[[maybe_unused]] JucePlugin&
PluginComponent::getPlugin() noexcept
{
  return *plugin;
}

[[maybe_unused]] JucePluginConstRef
PluginComponent::getPluginRef() const noexcept
{
  return plugin;
}

[[maybe_unused]] JucePluginRef
PluginComponent::getPluginRef() noexcept
{
  return plugin;
}

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PLUGIN_COMPONENT_HPP
