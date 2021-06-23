#include <blooper/internal/abstract/contextual.hpp>

BLOOPER_NAMESPACE_BEGIN

// Explicit instantiation

template class AnyAbstractCoreContext<StatefulTraits>;
template class AnyAbstractContext<StatefulTraits>;

template class AnyAbstractContextual<CoreContextualTraits>;
template class AnyAbstractContextual<ContextualTraits>;

template class AnyContextualBase<CoreContextualTraits>;
template class AnyContextualBase<ContextualTraits>;

BLOOPER_NAMESPACE_END
