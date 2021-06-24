#include <blooper/components/help/HelpWindow.hpp>

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

HelpWindow::~HelpWindow() = default;


// Window

[[maybe_unused]] void HelpWindow::closeButtonPressed()
{
  delete this;
}


// Show

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

  return window;
}

BLOOPER_NAMESPACE_END
