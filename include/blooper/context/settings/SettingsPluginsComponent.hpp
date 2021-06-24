#ifndef BLOOPER_SETTINGS_PLUGINS_COMPONENT_HPP
#define BLOOPER_SETTINGS_PLUGINS_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

class SettingsPluginsComponent : public CoreComponentBase
{
 public:
  BLOOPER_STATE_ID(SettingsPluginsComponent);


  struct Options
  {
  } options;

  explicit SettingsPluginsComponent(
      AbstractCoreContext& context,
      State                state,
      Options              options = {});


  // Component

 public:
  [[maybe_unused]] void resized() override;


  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsPluginsComponent);
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_SETTINGS_PLUGINS_COMPONENT_HPP
