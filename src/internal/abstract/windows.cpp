#include <blooper/internal/abstract/windows.hpp>

BLOOPER_NAMESPACE_BEGIN

// Explicit instantiation

template class AnyAbstractWindow<CoreWindowTraits>;
template class AnyAbstractWindow<WindowTraits>;

template class AnyWindowBase<CoreWindowTraits>;
template class AnyWindowBase<WindowTraits>;

BLOOPER_NAMESPACE_END
