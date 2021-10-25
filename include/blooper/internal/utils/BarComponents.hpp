#ifndef BLOOPER_BAR_COMPONENTS_HPP
#define BLOOPER_BAR_COMPONENTS_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>
#include <blooper/internal/abstract/juceTraits.hpp>

BLOOPER_UTIL_NAMESPACE_BEGIN

enum class BarPosition
{
  top,
  bottom
};

struct BarBuilderOptions
{
  BarPosition position = BarPosition::bottom;
  int         padding = 2;
  int         height = 0;
};

[[maybe_unused, nodiscard]] inline struct BarBuilder buildBar(
    JuceLookAndFeel&  lookAndFeel,
    JuceBounds        availableArea,
    BarBuilderOptions options = {}) noexcept;


struct BarBuilder
{
  template<typename... TComponents>
  [[maybe_unused]] inline auto addComponentsLeft(
      TComponents&... components) &&
  {
    static_assert(
        meta::and_(
            meta::true_c,
            isJuceComponent(BLOOPER_TYPEID(components))...),
        "All lvalues passed to BarBuilder must be components.");

    (this->addComponentLeft(components), ...);

    return *this;
  }

  template<typename... TComponents>
  [[maybe_unused]] inline auto addComponentsRight(
      TComponents&... components) &&
  {
    static_assert(
        meta::and_(
            meta::true_c,
            isJuceComponent(BLOOPER_TYPEID(components))...),
        "All lvalues passed to BarBuilder must be components.");

    (this->addComponentRight(components), ...);

    return *this;
  }

  [[maybe_unused]] inline auto finish() &&
  {
    return unusedArea;
  }


 private:
  [[maybe_unused, nodiscard]] inline BarBuilder(
      JuceLookAndFeel&  lookAndFeel,
      JuceBounds        availableArea,
      BarBuilderOptions options = {}) noexcept
      : lookAndFeel(lookAndFeel),
        options(move(options))
  {
    if (this->options.height <= 0)
    {
      this->options.height = lookAndFeel.getDefaultMenuBarHeight();
    }

    if (this->options.position == BarPosition::bottom)
    {
      this->availableArea =
          availableArea.removeFromBottom(
              this->options.height);
    }
    else
    {
      this->availableArea =
          availableArea.removeFromTop(
              this->options.height);
    }

    this->unusedArea = availableArea;
  }


  template<typename TComponent>
  inline auto addComponentRight(TComponent& component)
  {
    this->addComponent(
        component,
        availableArea.removeFromRight(
            this->getComponentWidth(component) +
            this->options.padding * 2));
  }

  template<typename TComponent>
  inline auto addComponentLeft(TComponent& component)
  {
    this->addComponent(
        component,
        availableArea.removeFromLeft(
            this->getComponentWidth(component) +
            this->options.padding * 2));
  }

  template<typename TComponent>
  inline auto getComponentWidth(TComponent& component)
  {
    if constexpr (std::is_same_v<TComponent, juce::TextButton>)
    {
      return lookAndFeel
          .getTextButtonWidthToFitText(
              component,
              availableArea.getHeight());
    }
    else if constexpr (std::is_same_v<TComponent, juce::ToggleButton>)
    {
      lookAndFeel.changeToggleButtonWidthToFitText(component);

      return component.getWidth();
    }
    else if constexpr (std::is_same_v<TComponent, juce::Label>)
    {
      return lookAndFeel
          .getLabelFont(component)
          .getStringWidth(component.getText());
    }
    else
    {
      return component.getWidth();
    }
  }

  template<typename TComponent>
  inline auto addComponent(TComponent& component, JuceBounds at)
  {
    component.setBounds(at.reduced(this->options.padding));
  }


  [[maybe_unused]] JuceLookAndFeel& lookAndFeel;
  [[maybe_unused]] JuceBounds       availableArea;

  [[maybe_unused]] BarBuilderOptions options;

  [[maybe_unused]] JuceBounds unusedArea;


  friend BarBuilder buildBar(
      JuceLookAndFeel&  lookAndFeel,
      JuceBounds        availableArea,
      BarBuilderOptions options) noexcept;
};


[[maybe_unused, nodiscard]] inline BarBuilder buildBar(
    JuceLookAndFeel&  lookAndFeel,
    JuceBounds        availableArea,
    BarBuilderOptions options) noexcept
{
  return {lookAndFeel, move(availableArea), move(options)};
}

BLOOPER_UTIL_NAMESPACE_END

#endif // BLOOPER_BAR_COMPONENTS_HPP
