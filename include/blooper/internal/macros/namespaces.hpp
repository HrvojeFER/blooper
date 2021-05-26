#ifndef BLOOPER_NAMESPACES_HPP
#define BLOOPER_NAMESPACES_HPP

// shortcuts

namespace te = tracktion_engine;


// main namespaces

#define BLOOPER_NAMESPACE_BEGIN \
  namespace blooper             \
  {
#define BLOOPER_NAMESPACE_END \
  } /* namespace blooper */

BLOOPER_NAMESPACE_BEGIN
BLOOPER_NAMESPACE_END

#define BLOOPER_ID_NAMESPACE_BEGIN \
  namespace blooper::id            \
  {
#define BLOOPER_ID_NAMESPACE_END \
  } /* namespace blooper::id */

BLOOPER_ID_NAMESPACE_BEGIN
BLOOPER_ID_NAMESPACE_END


// internal namespaces

#define BLOOPER_DEV_NAMESPACE_BEGIN \
  namespace blooper::dev            \
  {                                 \
  namespace                         \
  {
#define BLOOPER_DEV_NAMESPACE_END \
  } /* namespace */               \
  } /* namespace blooper::dev */

BLOOPER_DEV_NAMESPACE_BEGIN
BLOOPER_DEV_NAMESPACE_END


#define BLOOPER_EXT_NAMESPACE_BEGIN \
  namespace blooper::ext            \
  {                                 \
  namespace                         \
  {
#define BLOOPER_EXT_NAMESPACE_END \
  } /* namespace */               \
  } /* namespace blooper::ext */

BLOOPER_EXT_NAMESPACE_BEGIN
BLOOPER_EXT_NAMESPACE_END


#define BLOOPER_UTIL_NAMESPACE_BEGIN \
  namespace blooper::util            \
  {                                  \
  namespace                          \
  {
#define BLOOPER_UTIL_NAMESPACE_END \
  } /* namespace */                \
  } /* namespace blooper::util */

BLOOPER_UTIL_NAMESPACE_BEGIN
BLOOPER_UTIL_NAMESPACE_END


#define BLOOPER_META_NAMESPACE_BEGIN \
  namespace blooper::meta            \
  {                                  \
  using namespace env::syntax;       \
  using namespace env::meta;         \
  namespace                          \
  {
#define BLOOPER_META_NAMESPACE_END \
  } /* namespace */                \
  } /* namespace blooper::meta */

BLOOPER_META_NAMESPACE_BEGIN
BLOOPER_META_NAMESPACE_END

#define BLOOPER_TRAITS_NAMESPACE_BEGIN \
  namespace blooper::meta::traits      \
  {                                    \
  using namespace env::syntax;         \
  using namespace env::meta::traits;   \
  namespace                            \
  {
#define BLOOPER_TRAITS_NAMESPACE_END \
  } /* namespace */                  \
  } /* namespace blooper::meta::traits */

BLOOPER_TRAITS_NAMESPACE_BEGIN
BLOOPER_TRAITS_NAMESPACE_END

#endif //BLOOPER_NAMESPACES_HPP
