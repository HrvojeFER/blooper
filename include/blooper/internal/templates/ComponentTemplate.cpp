#include <blooper/__path__>

BLOOPER_NAMESPACE_BEGIN

__name__Component::__name__Component(
    AbstractContext&         context,
    State                    state,
    __name__ComponentOptions options)
    : ComponentBase(
          context,
          move(state)),
      options(move(options))
{
}

__name__Component::~__name__Component() = default;


// Component

void __name__Component::resized()
{
}

BLOOPER_NAMESPACE_END
