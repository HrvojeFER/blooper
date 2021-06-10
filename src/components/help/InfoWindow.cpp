#include <blooper/blooper.hpp>

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


// Window

[[maybe_unused]] void InfoWindow::closeButtonPressed()
{
  delete this;
}


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

  window->enterModalState(
      true,
      nullptr,
      true);

  return window;
}

BLOOPER_NAMESPACE_END
