#ifndef BLOOPER_PLUGINS_MENU_WINDOW_HPP
#define BLOOPER_PLUGINS_MENU_WINDOW_HPP

#include <blooper/internal/internal.hpp>

BLOOPER_NAMESPACE_BEGIN

class PluginsMenuWindow : public CoreWindowBase
{
 public:
  explicit PluginsMenuWindow(AbstractCoreContext& context);

 private:
};

void showPluginsMenu(AbstractCoreContext& context);

BLOOPER_NAMESPACE_END


#endif // BLOOPER_PLUGINS_MENU_WINDOW_HPP
