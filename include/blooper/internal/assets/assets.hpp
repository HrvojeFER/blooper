#ifndef BLOOPER_ASSETS_HPP
#define BLOOPER_ASSETS_HPP

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/abstract.hpp>

#include <blooper_binaries.hpp>

BLOOPER_ASSETS_NAMESPACE_BEGIN

using ResourceIndex = int;
using ResourceSize = size_t;
using Resource = const void*;

template<typename TConstructor>
using AssetType =
    decltype(std::declval<TConstructor>()(
        std::declval<Resource>(),
        std::declval<ResourceSize>()));

template<typename TId, typename TConstructor>
[[maybe_unused]] inline AssetType<TConstructor>
getAsset(TId                       id,
         TConstructor              constructor,
         AssetType<TConstructor>&& _default = {});


enum class DrawableAssetId : ResourceIndex
{
  playIcon [[maybe_unused]] = 0,
  repeatIcon [[maybe_unused]] = 1,
  pauseIcon [[maybe_unused]] = 2,
  stopIcon [[maybe_unused]] = 3,
  recordIcon [[maybe_unused]] = 4,

  mutedIcon [[maybe_unused]] = 5,
  lowVolumeIcon [[maybe_unused]] = 6,
  midVolumeIcon [[maybe_unused]] = 7,
  highVolumeIcon [[maybe_unused]] = 8,

  notImplementedIcon [[maybe_unused]] = 9,
};

[[maybe_unused]] inline auto getDrawableAssetUnsafe(ResourceIndex index)
{
  return getAsset(
      index,
      [](auto resource, auto size) {
        return JuceDrawable::createFromImageData(resource, size);
      });
}

[[maybe_unused]] inline auto getDrawableAsset(DrawableAssetId id)
{
  return getDrawableAssetUnsafe(static_cast<ResourceIndex>(id));
}


// resources for reference:

//const char* namedResourceList[] =
//    {
//        "_0_play_png",
//        "_1_repeat_png",
//        "_2_pause_png",
//        "_3_stop_png",
//        "_4_record_png",
//        "_5_muted_png",
//        "_6_low_volume_png",
//        "_7_mid_volume_png",
//        "_8_high_volume_png"
//    };

// JUCE adds a '_' to resource names, so the name format is:
//    _<index>_<name>_<file_format>

template<typename TId, typename TConstructor>
[[maybe_unused]] inline AssetType<TConstructor>
getAsset(TId                       id,
         TConstructor              constructor,
         AssetType<TConstructor>&& _default)
{
  static_assert(
      meta::traits::is_castable(
          meta::type_c<TId>,
          meta::type_c<ResourceIndex>),
      "Id passed to getAsset needs to be castable to a ResourceIndex.");

  static_assert(
      isInvokeable(
          meta::type_c<TConstructor>,
          meta::type_c<Resource>,
          meta::type_c<ResourceSize>),
      "Constructor passed getAsset has to be "
      "an Invokable with a Resource and a ResourceSize.");


  auto        resourceIndex = static_cast<ResourceIndex>(id);
  const char* resourceName = nullptr;

  for (int i = 0; i < binaries::namedResourceListSize; ++i)
  {
    auto currentResource = binaries::namedResourceList[i];
    auto currentResourceIndexChars = currentResource + 1;
    int  currentResourceIndex = -1;

    std::stringstream(currentResourceIndexChars) >> currentResourceIndex;
    if (currentResourceIndex == resourceIndex)
    {
      resourceName = currentResource;
      break;
    }
  }

  if (!resourceName) return std::forward<AssetType<TConstructor>>(_default);


  auto size = 0;
  auto resource = binaries::getNamedResource(resourceName, size);

  return constructor(resource, size);
}

BLOOPER_ASSETS_NAMESPACE_END

#endif // BLOOPER_ASSETS_HPP
