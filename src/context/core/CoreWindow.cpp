#include <blooper/context/core/CoreWindow.hpp>


BLOOPER_NAMESPACE_BEGIN

CoreWindow::CoreWindow(CoreContext& context)
    : context(context)
{
}

CoreWindow::~CoreWindow() = default;

BLOOPER_NAMESPACE_END
