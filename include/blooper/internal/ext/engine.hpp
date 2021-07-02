#ifndef BLOOPER_EXT_ENGINE_HPP
#define BLOOPER_EXT_ENGINE_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>

BLOOPER_EXT_NAMESPACE_BEGIN

[[maybe_unused]] inline bool isMonitoring(te::Engine& engine)
{
  auto& manager = engine.getDeviceManager();

  for (int i = 0; i < manager.getNumInputDevices(); ++i)
    if (auto device = manager.getInputDevice(i);
        device->isEndToEndEnabled())
      return true;

  return false;
}

[[maybe_unused]] inline void toggleMonitoring(te::Engine& engine)
{
  auto& manager = engine.getDeviceManager();

  for (int i = 0; i < manager.getNumInputDevices(); ++i)
    if (auto device = manager.getInputDevice(i))
      device->flipEndToEnd();
}

BLOOPER_EXT_NAMESPACE_END

#endif // BLOOPER_EXT_ENGINE_HPP
