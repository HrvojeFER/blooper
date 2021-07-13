#include <blooper/__path__>

BLOOPER_NAMESPACE_BEGIN

__name__Window::__name__Window(
    AbstractContext&      context,
    State                 state,
    __name__WindowOptions options)
    : WindowBase(
          "__name__",
          context,
          move(state)),
      options(move(options))
{
}

__name__Window::~__name__Window() = default;


// Show

__name__Window* show__name__(
    AbstractContext&      context,
    __name__WindowOptions options)
{
  auto window =
      new __name__Window(
          context,
          context.getState().getOrCreateChildWithName(
              __name__Window::stateId,
              nullptr),
          move(options));

  return window;
}

BLOOPER_NAMESPACE_END
