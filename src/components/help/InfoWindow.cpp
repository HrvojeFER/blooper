#include <blooper/components/help/InfoWindow.hpp>

BLOOPER_NAMESPACE_BEGIN

InfoWindow::InfoWindow(
    AbstractContext& context,
    State            state,
    Options          options)
    : WindowBase(
          "Info",
          context,
          move(state)),
      options(move(options))
{
}

InfoWindow::~InfoWindow() = default;


// Window

[[maybe_unused]] void InfoWindow::closeButtonPressed()
{
  delete this;
}


// Show

[[maybe_unused]] InfoWindow* showInfo(
    AbstractContext&    context,
    InfoWindow::Options options)
{
  auto window =
      new InfoWindow(
          context,
          context.getState().getOrCreateChildWithName(
              InfoWindow::stateId,
              nullptr),
          move(options));

  return window;
}

BLOOPER_NAMESPACE_END
