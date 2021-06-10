#include <blooper/blooper.hpp>

BLOOPER_UTIL_NAMESPACE_BEGIN

// EditTrack


// Plugin


// Parameter


// instances

#define _DECLARE_SELECTABLE_CLASS(_class, _object)  \
  te::SelectableClass::ClassInstance<               \
      _class,                                       \
      _object> /* NOLINTNEXTLINE(cert-err58-cpp) */ \
      _class##Instance

_DECLARE_SELECTABLE_CLASS(EditTrackSelectableClass, EditTrack);
_DECLARE_SELECTABLE_CLASS(PluginSelectableClass, te::Plugin);
_DECLARE_SELECTABLE_CLASS(ParameterSelectableClass, te::AutomatableParameter);

#undef _DECLARE_SELECTABLE_CLASS

BLOOPER_UTIL_NAMESPACE_END
