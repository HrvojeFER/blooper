#ifndef BLOOPER_ABSTRACT_WINDOWS_HPP
#define BLOOPER_ABSTRACT_WINDOWS_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>

#include <blooper/internal/abstract/id.hpp>

#include <blooper/internal/abstract/prelude.hpp>
#include <blooper/internal/abstract/meta.hpp>
#include <blooper/internal/abstract/traits.hpp>
#include <blooper/internal/abstract/juceTraits.hpp>

#include <blooper/internal/abstract/stateful.hpp>
#include <blooper/internal/abstract/contextual.hpp>

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

[[maybe_unused]] inline constexpr auto isAnyWindow =
    meta::satisfies_all(
        isJuceWindow,
        isAnyContextual,
        isAnyStateful);

template<typename TWindowTraits>
[[maybe_unused]] inline constexpr auto isAnyWindowBase =
    meta::satisfies_all(
        meta::reverse_partial(
            meta::traits::is_constructible,
            meta::type_c<JuceString>,
            meta::type_c<typename TWindowTraits::
                             contextualTraits::contextType&>,
            meta::type_c<typename TWindowTraits::
                             statefulTraits::stateType>),
        isAnyWindow);

[[maybe_unused]] inline constexpr auto isCoreWindow =
    meta::satisfies_all(
        isAnyWindow,
        isCoreContextual,
        isStateful);

[[maybe_unused]] inline constexpr auto isWindow =
    meta::satisfies_all(
        isAnyWindow,
        isContextual,
        isStateful);

[[maybe_unused]] inline constexpr auto isCoreWindowBase =
    meta::satisfies_all(
        meta::reverse_partial(
            meta::traits::is_constructible,
            meta::type_c<JuceString>,
            meta::type_c<AbstractCoreContext&>,
            meta::type_c<State>),
        isCoreWindow);

[[maybe_unused]] inline constexpr auto isWindowBase =
    meta::satisfies_all(
        meta::reverse_partial(
            meta::traits::is_constructible,
            meta::type_c<JuceString>,
            meta::type_c<AbstractContext&>,
            meta::type_c<State>),
        isWindow);


[[maybe_unused]] inline constexpr auto isAnyWindowTraits =
    meta::attribute(
        [](auto&& toCheck)
            -> decltype(meta::and_(
                isAnyContextualTraits(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     contextualTraits>),
                isAnyStatefulTraits(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     statefulTraits>),
                isJuceWindow(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     juceBaseType>),

                isAnyWindow(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     abstractType>),
                isAnyWindowBase<std::decay_t<decltype(toCheck)>>(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     baseType>),

                meta::traits::is_base_of(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     juceBaseType>,
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     abstractType>),
                meta::traits::is_base_of(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     abstractType>,
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     baseType>))) {}) ^
    meta::after ^
    meta::check(
        [](auto&& toCheck)
            -> decltype(meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         contextualTraits>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         statefulTraits>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         juceBaseType>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         abstractType>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         baseType>) {});

template<typename TWindowTraits>
class AnyAbstractWindow;

template<typename TWindowTraits>
class AnyWindowBase;

template<typename TWindowTraits>
class JuceStateWindowBase;

template<
    typename TContextualTraits,
    typename TStatefulTraits,
    typename TJuceBase = JuceWindow,
    typename = void>
struct [[maybe_unused]] AnyWindowTraits
{
  static_assert(
      isAnyContextualTraits(meta::type_c<TContextualTraits>),
      "AnyWindowTraits requires AnyContext.");

  static_assert(
      isAnyStatefulTraits(meta::type_c<TStatefulTraits>),
      "AnyWindowTraits requires AnyState.");

  static_assert(
      isJuceWindow(meta::type_c<TJuceBase>),
      "AnyWindowTraits requires a JuceWindow.");


  using contextualTraits [[maybe_unused]] = TContextualTraits;
  using statefulTraits [[maybe_unused]] = TStatefulTraits;

  using juceBaseType [[maybe_unused]] = TJuceBase;

  using abstractType [[maybe_unused]] =
      AnyAbstractWindow<AnyWindowTraits>;

  using baseType [[maybe_unused]] =
      AnyWindowBase<AnyWindowTraits>;
};

template<
    typename TContextualTraits,
    typename TStatefulTraits,
    typename TJuceBase>
struct [[maybe_unused]] AnyWindowTraits<
    TContextualTraits,
    TStatefulTraits,
    TJuceBase,
    std::enable_if_t<isJuceState(
        meta::type_c<typename TStatefulTraits::stateType>)>>
{
  static_assert(
      isAnyContextualTraits(meta::type_c<TContextualTraits>),
      "AnyWindowTraits requires AnyContext.");

  static_assert(
      isJuceWindow(meta::type_c<TJuceBase>),
      "AnyWindowTraits requires a JuceWindow.");


  using contextualTraits [[maybe_unused]] = TContextualTraits;
  using statefulTraits [[maybe_unused]] = StatefulTraits;

  using juceBaseType [[maybe_unused]] = TJuceBase;

  using abstractType [[maybe_unused]] =
      AnyAbstractWindow<AnyWindowTraits>;

  using baseType [[maybe_unused]] =
      JuceStateWindowBase<AnyWindowTraits>;
};


template<typename TWindowTraits>
class [[maybe_unused]] AnyAbstractWindow :
    public virtual TWindowTraits::contextualTraits::abstractType,
    public virtual TWindowTraits::statefulTraits::abstractType,
    public TWindowTraits::juceBaseType
{
  using abstractContextualType [[maybe_unused]] =
      typename TWindowTraits::contextualTraits::abstractType;

  static_assert(
      isAnyContextual(meta::type_c<abstractContextualType>),
      "AnyAbstractWindow requires a Contextual.");


  using abstractStatefulType [[maybe_unused]] =
      typename TWindowTraits::statefulTraits::abstractType;

  static_assert(
      isAnyStateful(meta::type_c<abstractStatefulType>),
      "AnyAbstractWindow requires a Stateful.");


  using juceBaseType [[maybe_unused]] =
      typename TWindowTraits::juceBaseType;

  static_assert(
      isJuceWindow(meta::type_c<juceBaseType>),
      "AnyAbstractWindow requires a JuceWindow.");


 public:
  [[maybe_unused]] inline AnyAbstractWindow()
      : juceBaseType({}, {}, {})
  {
  }

  [[maybe_unused]] inline ~AnyAbstractWindow() override = default;
};

template<typename TWindowTraits>
class [[maybe_unused]] AnyWindowBase :
    public virtual TWindowTraits::abstractType,
    public TWindowTraits::contextualTraits::baseType,
    public TWindowTraits::statefulTraits::baseType
{
  using abstractType [[maybe_unused]] =
      typename TWindowTraits::abstractType;

  static_assert(
      isAnyWindow(meta::type_c<abstractType>),
      "AnyWindowBase requires AnyWindow.");


  using contextType [[maybe_unused]] =
      typename TWindowTraits::contextualTraits::contextType;

  static_assert(
      isAnyContext(meta::type_c<contextType>),
      "AnyWindowBase requires AnyContext.");

  using contextualBaseType [[maybe_unused]] =
      typename TWindowTraits::contextualTraits::baseType;

  static_assert(
      isAnyContextualBase<typename TWindowTraits::contextualTraits>(
          meta::type_c<contextualBaseType>),
      "AnyWindowBase requires AnyContextualBase.");


  using stateType [[maybe_unused]] =
      typename TWindowTraits::statefulTraits::stateType;

  static_assert(
      isAnyState(meta::type_c<stateType>),
      "AnyWindowBase requires AnyState.");

  using statefulBaseType [[maybe_unused]] =
      typename TWindowTraits::statefulTraits::baseType;

  static_assert(
      isAnyStatefulBase<typename TWindowTraits::statefulTraits>(
          meta::type_c<statefulBaseType>),
      "AnyWindowBase requires AnyStatefulBase.");


 public:
  [[maybe_unused]] inline explicit AnyWindowBase(
      JuceString   name,
      contextType& context,
      stateType    state)
      : contextualBaseType(context),
        statefulBaseType(move(state))
  {
    this->setName(move(name));


    const auto monitorBounds = this->getParentMonitorArea();
    const auto monitorWidth = monitorBounds.getWidth();
    const auto monitorHeight = monitorBounds.getHeight();
    this->setCentrePosition(monitorWidth / 2, monitorHeight / 2);
    this->setSize(monitorWidth / 2, monitorHeight / 2);

    this->setResizable(
        true,
        false);


    this->setTitleBarButtonsRequired(
        juce::DocumentWindow::allButtons,
        false);

    this->setTitleBarTextCentred(true);


    if constexpr (
        decltype(isCoreContext(
            meta::typeid_(this->getContext()))){})
    {
      auto& lookAndFeel = this->getContext().getLookAndFeel();

      this->setLookAndFeel(
          std::addressof(lookAndFeel));

      this->setBackgroundColour(
          lookAndFeel.findColour(
              juce::ResizableWindow::backgroundColourId));
    }
  }


  // Window

 protected:
  [[maybe_unused]] void closeButtonPressed() override
  {
    delete this;
  }
};

template<typename TWindowTraits>
class [[maybe_unused]] JuceStateWindowBase :
    public AnyWindowBase<TWindowTraits>
{
  using baseType [[maybe_unused]] = AnyWindowBase<TWindowTraits>;

  static_assert(
      isAnyWindowBase<TWindowTraits>(meta::type_c<baseType>),
      "StateWindowBase requires AnyWindowBase.");


  using contextType [[maybe_unused]] =
      typename TWindowTraits::contextualTraits::contextType;

  static_assert(
      isAnyContext(meta::type_c<contextType>),
      "StateWindowBase requires AnyContext.");


  using stateType [[maybe_unused]] =
      typename TWindowTraits::statefulTraits::stateType;

  static_assert(
      isJuceState(meta::type_c<stateType>),
      "AnyWindowBase requires JuceState.");


 public:
  [[maybe_unused]] inline explicit JuceStateWindowBase(
      JuceString   name,
      contextType& context,
      stateType    state)
      : baseType(
            move(name),
            context,
            move(state)),

        windowState(
            this->getState(),
            id::windowState,
            nullptr,
            "")
  {
    if (windowState->isNotEmpty())
      this->restoreWindowStateFromString(windowState);
  }

  ~JuceStateWindowBase() override
  {
    windowState.setValue(
        this->getWindowStateAsString(),
        nullptr);
  }


 protected:
  JuceStateValue<JuceString> windowState;
};


using CoreWindowTraits =
    AnyWindowTraits<
        CoreContextualTraits,
        StatefulTraits,
        JuceWindow>;

using AbstractCoreWindow = CoreWindowTraits::abstractType;
using CoreWindowBase = CoreWindowTraits::baseType;

BLOOPER_STATIC_ASSERT(
    isAnyWindowTraits(meta::type_c<CoreWindowTraits>),
    "AbstractCoreWindow must satisfy CoreWindow.");

BLOOPER_STATIC_ASSERT(
    isCoreWindow(meta::type_c<AbstractCoreWindow>),
    "AbstractCoreWindow must satisfy CoreWindow.");

BLOOPER_STATIC_ASSERT(
    isCoreWindowBase(meta::type_c<CoreWindowBase>),
    "CoreWindow must satisfy CoreWindowBase.");

using WindowTraits =
    AnyWindowTraits<
        ContextualTraits,
        StatefulTraits,
        JuceWindow>;

using AbstractWindow = WindowTraits::abstractType;
using WindowBase = WindowTraits::baseType;

BLOOPER_STATIC_ASSERT(
    isAnyWindowTraits(meta::type_c<WindowTraits>),
    "AbstractCoreWindow must satisfy CoreWindow.");

BLOOPER_STATIC_ASSERT(
    isWindow(meta::type_c<AbstractWindow>),
    "AbstractWindow must satisfy Window.");

BLOOPER_STATIC_ASSERT(
    isWindowBase(meta::type_c<WindowBase>),
    "Window must satisfy WindowBase.");


// Explicit instantiation

extern template class AnyAbstractWindow<CoreWindowTraits>;
extern template class AnyAbstractWindow<WindowTraits>;

extern template class AnyWindowBase<CoreWindowTraits>;
extern template class AnyWindowBase<WindowTraits>;

BLOOPER_NAMESPACE_END

#endif // BLOOPER_ABSTRACT_WINDOWS_HPP
