#include <blooper/components/dev/DevWindow.hpp>

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

DevWindow::~DevWindow() = default;


// Window

[[maybe_unused]] void DevWindow::closeButtonPressed()
{
  delete this;
}


// Show

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

  return window;
}

BLOOPER_NAMESPACE_END
