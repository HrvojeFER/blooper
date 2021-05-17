#ifndef BLOOPER_PLUGINS_MENU_WINDOW_HPP
#define BLOOPER_PLUGINS_MENU_WINDOW_HPP


#include <blooper/fwd.hpp>

#include <blooper/context/core/core.hpp>


BLOOPER_NAMESPACE_BEGIN

class PluginsMenuWindow : public CoreWindow
{
public:
    explicit PluginsMenuWindow(CoreContext& context);

private:
};

BLOOPER_NAMESPACE_END


#endif // BLOOPER_PLUGINS_MENU_WINDOW_HPP
