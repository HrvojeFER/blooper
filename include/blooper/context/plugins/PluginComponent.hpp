#ifndef BLOOPER_PLUGIN_COMPONENT_HPP
#define BLOOPER_PLUGIN_COMPONENT_HPP

#include <blooper/internal/abstract/abstract.hpp>

BLOOPER_NAMESPACE_BEGIN

class PluginComponent : public CoreComponentBase
{
 public:
  using PluginRef = te::Plugin::Ptr;


  [[nodiscard]] explicit PluginComponent(
      AbstractCoreContext& context,
      PluginRef            plugin);


  [[nodiscard]] inline const te::Plugin& getPlugin() const noexcept;


  void resized() override;


 private:
  [[nodiscard]] inline te::Plugin& getPlugin() noexcept;


  PluginRef plugin;


  class Button : public juce::TextButton
  {
   public:
    explicit Button(PluginComponent* parent);

    void clicked(const juce::ModifierKeys& modifiers) override;


   private:
    PluginComponent* parent;

  } button;
};


const te::Plugin& PluginComponent::getPlugin() const noexcept
{
  return *plugin;
}

te::Plugin& PluginComponent::getPlugin() noexcept
{
  return *plugin;
}

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PLUGIN_COMPONENT_HPP
