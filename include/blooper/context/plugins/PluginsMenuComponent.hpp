#ifndef BLOOPER_PLUGINS_MENU_COMPONENT_HPP
#define BLOOPER_PLUGINS_MENU_COMPONENT_HPP

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

class PluginsMenuComponent : public CoreComponentBase
{
 public:
  BLOOPER_STATE_ID(PluginsMenuComponent);


  struct Options
  {
  } options;

  explicit PluginsMenuComponent(
      AbstractCoreContext& context,
      State                state,
      Options              options = {});


 private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginsMenuComponent)
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PLUGINS_MENU_COMPONENT_HPP
