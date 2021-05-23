#ifndef BLOOPER_EXT_PLUGIN_HPP
#define BLOOPER_EXT_PLUGIN_HPP

#include <blooper/internal/macros/macros.hpp>

BLOOPER_EXT_NAMESPACE_BEGIN

[[maybe_unused]] inline bool isDPIAware(te::Plugin&)
{
  // You should keep a DB of if plugins are DPI aware or not and recall
  // that value here.
  // You should let the user toggle the value if the plugin appears tiny.
  return true;
}

BLOOPER_EXT_NAMESPACE_END

#endif // BLOOPER_EXT_PLUGIN_HPP
