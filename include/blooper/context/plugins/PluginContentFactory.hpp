#ifndef BLOOPER_PLUGIN_CONTENT_FACTORY_HPP
#define BLOOPER_PLUGIN_CONTENT_FACTORY_HPP
#pragma once

#include <blooper/internal/abstract/plugins.hpp>

BLOOPER_NAMESPACE_BEGIN

struct PluginContentOptions
{
};

[[maybe_unused, nodiscard]] std::unique_ptr<AbstractPluginContentComponent>
createPluginContent(
    AbstractCoreContext& context,
    JucePluginRef        plugin,
    PluginContentOptions options = {});


class NotImplementedPluginContentComponent final :
    public PluginContentComponentBase
{
 public:
  BLOOPER_STATE_ID(NotImplementedPluginContentComponent);


  struct Options
  {
  } options;

  [[maybe_unused]] explicit NotImplementedPluginContentComponent(
      AbstractCoreContext& context,
      State                state,
      JucePluginRef        plugin,
      Options              options);


  [[maybe_unused, nodiscard]] inline const JuceConstrainer*
  getConstrainer() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceConstrainer*
  getConstrainer() noexcept final;


  [[maybe_unused, nodiscard]] inline bool
  checkIsResizable() const noexcept final;


  [[maybe_unused]] void recreate() final;


  void resized() final;


 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(
      NotImplementedPluginContentComponent)
};


struct ExternalPluginContentComponent final :
    public ExternalPluginContentComponentBase
{
 public:
  BLOOPER_STATE_ID(ExternalPluginContentComponent);


  struct Options
  {
  } options;

  [[maybe_unused]] explicit ExternalPluginContentComponent(
      AbstractCoreContext&  context,
      State                 state,
      JuceExternalPluginRef plugin,
      Options               options = {});


  [[maybe_unused, nodiscard]] inline const JuceConstrainer*
  getConstrainer() const noexcept final;

  [[maybe_unused, nodiscard]] inline JuceConstrainer*
  getConstrainer() noexcept final;


  [[maybe_unused, nodiscard]] inline bool
  checkIsResizable() const noexcept final;


  [[maybe_unused]] void recreate() final;


  void resized() final;

  void childBoundsChanged(JuceComponent* component) final;


 private:
  std::unique_ptr<JucePluginContent> content;


  void resizeToFitContent(bool force = false);


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ExternalPluginContentComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PLUGIN_CONTENT_FACTORY_HPP
