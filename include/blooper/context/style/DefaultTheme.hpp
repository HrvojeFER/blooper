#ifndef BLOOPER_DEFAULT_THEME_HPP
#define BLOOPER_DEFAULT_THEME_HPP
#pragma once

#include <blooper/internal/macros/namespaces.hpp>
#include <blooper/internal/abstract/juceTraits.hpp>
#include <blooper/internal/utils/style.hpp>

BLOOPER_NAMESPACE_BEGIN

class [[maybe_unused]] DefaultTheme final :
    public virtual AbstractTheme
{
 public:
  [[maybe_unused, nodiscard]] bool
  hasColour(JuceColourId id) const final;

  [[maybe_unused, nodiscard]] JuceColour
  getColour(JuceColourId id) const final;
};


BLOOPER_NAMESPACE_END

#endif // BLOOPER_DEFAULT_THEME_HPP
