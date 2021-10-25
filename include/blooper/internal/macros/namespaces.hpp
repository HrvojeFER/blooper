#ifndef BLOOPER_NAMESPACES_HPP
#define BLOOPER_NAMESPACES_HPP
#pragma once

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

#define BLOOPER_EXT_NAMESPACE_BEGIN \
  namespace blooper::ext            \
  {
#define BLOOPER_EXT_NAMESPACE_END \
  } /* namespace blooper::ext */

BLOOPER_EXT_NAMESPACE_BEGIN
BLOOPER_EXT_NAMESPACE_END

#define BLOOPER_UTIL_NAMESPACE_BEGIN \
  namespace blooper::util            \
  {
#define BLOOPER_UTIL_NAMESPACE_END \
  } /* namespace blooper::util */

BLOOPER_UTIL_NAMESPACE_BEGIN
BLOOPER_UTIL_NAMESPACE_END

#define BLOOPER_DETAIL_NAMESPACE_BEGIN \
  namespace blooper::detail            \
  {
#define BLOOPER_DETAIL_NAMESPACE_END \
  } /* namespace blooper::detail */

BLOOPER_DETAIL_NAMESPACE_BEGIN
BLOOPER_DETAIL_NAMESPACE_END

#define BLOOPER_TEST_NAMESPACE_BEGIN \
  namespace blooper::test            \
  {
#define BLOOPER_TEST_NAMESPACE_END \
  } /* namespace blooper::test */

BLOOPER_TEST_NAMESPACE_BEGIN
BLOOPER_TEST_NAMESPACE_END

#define BLOOPER_ASSETS_NAMESPACE_BEGIN \
  namespace blooper::assets            \
  {
#define BLOOPER_ASSETS_NAMESPACE_END \
  } /* namespace blooper::assets */

BLOOPER_ASSETS_NAMESPACE_BEGIN
BLOOPER_ASSETS_NAMESPACE_END


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

#define BLOOPER_STD_NAMESPACE_BEGIN    \
  /* NOLINTNEXTLINE(cert-dcl58-cpp) */ \
  namespace std                        \
  {
#define BLOOPER_STD_NAMESPACE_END \
  } /* namespace std */

BLOOPER_STD_NAMESPACE_BEGIN
BLOOPER_STD_NAMESPACE_END

#endif //BLOOPER_NAMESPACES_HPP
