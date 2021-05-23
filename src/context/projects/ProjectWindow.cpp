#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

ProjectWindow::ProjectWindow(AbstractContext& context)
    : WindowBase("Project", context)
{
  setContentOwned(
      new ProjectComponent(context),
      true);
}


void showProject(AbstractContext& context)
{
  auto window = new ProjectWindow(context);

  window->enterModalState(
      true,
      nullptr,
      true);
}

BLOOPER_NAMESPACE_END
