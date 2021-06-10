#include <blooper/blooper.hpp>

BLOOPER_NAMESPACE_BEGIN

HelpWindow::HelpWindow(
    AbstractContext& context,
    State            state,
    Options          options)
    : WindowBase(
          "Help",
          context,
          move(state)),
      options(move(options))
{
}


// Window

[[maybe_unused]] void HelpWindow::closeButtonPressed()
{
  delete this;
}


[[maybe_unused]] HelpWindow* showHelp(
    AbstractContext&    context,
    HelpWindow::Options options)
{
  auto window =
      new HelpWindow(
          context,
          context.getState().getOrCreateChildWithName(
              HelpWindow::stateId,
              nullptr),
          move(options));

  window->enterModalState(
      true,
      nullptr,
      true);

  return window;
}

BLOOPER_NAMESPACE_END
