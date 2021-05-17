#include <blooper/blooper.hpp>


BLOOPER_NAMESPACE_BEGIN

EngineBehaviour::EngineBehaviour(AbstractCoreContext& context)
    : context(context)
{ }

EngineBehaviour::~EngineBehaviour() = default;

BLOOPER_NAMESPACE_END
