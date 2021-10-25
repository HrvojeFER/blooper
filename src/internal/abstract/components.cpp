#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

// Explicit instantiation

template class AnyAbstractComponent<CoreComponentTraits>;
template class AnyAbstractComponent<ComponentTraits>;

template class AnyComponentBase<CoreComponentTraits>;
template class AnyComponentBase<ComponentTraits>;

BLOOPER_NAMESPACE_END
