#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

DevWindow::DevWindow(
    AbstractContext& context,
    State            state,
    Options          options)
    : WindowBase(
          "Dev",
          context,
          move(state)),
      options(move(options))
{
}


// Window

[[maybe_unused]] void DevWindow::closeButtonPressed()
{
  delete this;
}


[[maybe_unused]] DevWindow* showDev(
    AbstractContext&   context,
    DevWindow::Options options)
{
  auto window =
      new DevWindow(
          context,
          context.getState().getOrCreateChildWithName(
              DevWindow::stateId,
              nullptr),
          move(options));

  window->enterModalState(
      true,
      nullptr,
      true);

  return window;
}

BLOOPER_NAMESPACE_END
