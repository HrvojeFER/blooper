#ifndef BLOOPER_TAKE_COMPONENT_HPP
#define BLOOPER_TAKE_COMPONENT_HPP
#pragma once

#include <blooper/internal/abstract/time.hpp>
#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

struct TakeComponentOptions
{
  bool shouldResizeItself = true;
  bool shouldResizeParentWhenResized = true;

  AbstractTimeXConverter* timeXConverter{nullptr};
};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_TAKE_COMPONENT_HPP
