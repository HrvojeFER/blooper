#ifndef BLOOPER_ASSETS_HPP
#define BLOOPER_ASSETS_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>

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


enum class IconAssetId : ResourceIndex
{
  // Special

  notImplementedIcon [[maybe_unused]] = 0,


  // App

  quit [[maybe_unused]] = 1000,
  saveProject [[maybe_unused]] = 1001,
  saveEdit [[maybe_unused]] = 1005, // TODO
  saveAndQuit [[maybe_unused]] = 1002,
  saveAll [[maybe_unused]] = 1003,
  saveAs [[maybe_unused]] = 1004,

  settings [[maybe_unused]] = 1100,
  projectSettings [[maybe_unused]] = 1101,
  deviceManager [[maybe_unused]] = 1102,

  openProject [[maybe_unused]] = 1200, // TODO

  help [[maybe_unused]] = 1300,
  info [[maybe_unused]] = 1301,
  dev [[maybe_unused]] = 1302,

  toggleMasterTrackPanel [[maybe_unused]] = 1400,
  toggleControlSurfacePanel [[maybe_unused]] = 1401,
  toggleBrowserPanel [[maybe_unused]] = 1402,

  menu [[maybe_unused]] = 1500, // TODO


  // Edit

  del [[maybe_unused]] = 2000,
  cut [[maybe_unused]] = 2001,
  copy [[maybe_unused]] = 2002,
  paste [[maybe_unused]] = 2003,

  addTrack [[maybe_unused]] = 2100,
  addPlugin [[maybe_unused]] = 2101,
  addEdit [[maybe_unused]] = 2102, // TODO

  undo [[maybe_unused]] = 2200,
  redo [[maybe_unused]] = 2201,


  // Transport

  play [[maybe_unused]] = 3000,
  pause [[maybe_unused]] = 3001,
  stop [[maybe_unused]] = 3002,
  record [[maybe_unused]] = 3003,
  scheduledPlay [[maybe_unused]] = 3004,   // TODO
  scheduledRecord [[maybe_unused]] = 3005, // TODO

  monitor [[maybe_unused]] = 3100, // TODO


  // Track

  muteTrack [[maybe_unused]] = 4001,
  soloTrack [[maybe_unused]] = 4002,
  armTrack [[maybe_unused]] = 4003,

  plugin [[maybe_unused]] = 4100,

  cycleTrackMode [[maybe_unused]] = 4200,
  syncTrackMode [[maybe_unused]] = 4201, // TODO
  oneShotTrackMode [[maybe_unused]] = 4202,
  freeTrackMode [[maybe_unused]] = 4203,

  cycleTrackInterval [[maybe_unused]] = 4400, // TODO

  clearTrack [[maybe_unused]] = 4300, // TODO


  // Parameter

  nudgeUp [[maybe_unused]] = 5000,
  nudgeDown [[maybe_unused]] = 5001,

  elevate [[maybe_unused]] = 5100,
};

[[maybe_unused]] inline auto getIconAssetUnsafe(ResourceIndex index)
{
  if (auto asset = getAsset(
          index,
          [](auto resource, auto size) {
            return JuceDrawable::createFromImageData(resource, size);
          }))
    return std::move(asset);

  return getIconAssetUnsafe(
      static_cast<ResourceIndex>(
          IconAssetId::notImplementedIcon));
}

[[maybe_unused]] inline auto getIconAsset(IconAssetId id)
{
  return getIconAssetUnsafe(static_cast<ResourceIndex>(id));
}


// JUCE adds a '_' to resource names, so the name format is:
//    _<index>_<name>_<file_format>

template<typename TId, typename TConstructor>
[[maybe_unused]] inline AssetType<TConstructor>
getAsset(TId                       id,
         TConstructor              constructor,
         AssetType<TConstructor>&& _default)
{
  static_assert(
      BLOOPER_TYPEID(id) ^
          meta::traits::is_statically_castable_into ^
          meta::type_c<ResourceIndex>,
      "Id passed to getAsset needs to be "
      "statically castable into a ResourceIndex.");

  static_assert(
      isInvokable(
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
