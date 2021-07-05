#ifndef BLOOPER_STATEFUL_HPP
#define BLOOPER_STATEFUL_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>

#include <blooper/internal/abstract/prelude.hpp>
#include <blooper/internal/abstract/meta.hpp>
#include <blooper/internal/abstract/traits.hpp>

#include <blooper/internal/abstract/juceTraits.hpp>

BLOOPER_NAMESPACE_BEGIN

[[maybe_unused]] inline constexpr auto isAnyState =
    meta::always(meta::true_c);

using State [[maybe_unused]] = JuceState;
using StateIdentifier [[maybe_unused]] = JuceStateIdentifier;
using StateListener [[maybe_unused]] = JuceStateListener;


[[maybe_unused]] inline constexpr auto isAnyStateful =
    meta::attribute(
        [](auto&& toCheck)
            -> decltype(isAnyState(meta::typeid_(toCheck.getState()))) {}) ^
    meta::after ^
    meta::check(
        [](auto&& toCheck)
            -> decltype(toCheck.getState()) {});

template<typename TStatefulTraits>
[[maybe_unused]] inline constexpr auto isAnyStatefulBase =
    meta::satisfies_all(
        meta::reverse_partial(
            meta::traits::is_constructible,
            meta::type_c<typename TStatefulTraits::stateType>),
        isAnyStateful);

[[maybe_unused]] inline constexpr auto isStateful =
    meta::attribute(
        [](auto&& toCheck)
            -> decltype(meta::and_(
                meta::traits::is_same(
                    meta::typeid_(toCheck.getState()),
                    meta::type_c<State>))) {}) ^
    meta::after ^ isAnyStateful;

[[maybe_unused]] inline constexpr auto isStatefulBase =
    meta::satisfies_all(
        meta::reverse_partial(
            meta::traits::is_constructible,
            meta::type_c<State>),
        isStateful);


[[maybe_unused]] inline constexpr auto isAnyStatefulTraits =
    meta::attribute(
        [](auto&& toCheck)
            -> decltype(meta::and_(
                isAnyState(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     stateType>),
                isAnyStateful(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     abstractType>),
                isAnyStatefulBase<std::decay_t<decltype(toCheck)>>(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     baseType>))) {}) ^
    meta::after ^
    meta::check(
        [](auto&& toCheck)
            -> decltype(meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         stateType>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         abstractType>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         baseType>) {});

template<typename TStatefulTraits>
class AnyAbstractStateful;

template<typename TStatefulTraits>
class AnyStatefulBase;

template<typename TStatefulTraits>
class JuceStatefulBase;

template<typename TState, typename = void>
struct AnyStatefulTraits
{
  static_assert(
      isAnyState(meta::type_c<TState>),
      "AnyStatefulTraits requires AnyState.");


  using stateType = TState;

  using abstractType = AnyAbstractStateful<AnyStatefulTraits>;
  using baseType = AnyStatefulBase<AnyStatefulTraits>;
};

template<typename TState>
struct AnyStatefulTraits<
    TState,
    std::enable_if_t<isJuceState(meta::type_c<TState>)>>
{
  using stateType = State;

  using abstractType = AnyAbstractStateful<AnyStatefulTraits>;
  using baseType = JuceStatefulBase<AnyStatefulTraits>;
};


template<typename TStatefulTraits>
class [[maybe_unused]] AnyAbstractStateful
{
  using stateType [[maybe_unused]] = typename TStatefulTraits::stateType;

  static_assert(
      isAnyState(meta::type_c<stateType>),
      "AbstractAnyStateful requires AnyState.");


 public:
  [[maybe_unused]] inline AnyAbstractStateful() = default;

  [[maybe_unused]] virtual inline ~AnyAbstractStateful() = default;


  [[maybe_unused, nodiscard]] virtual inline const stateType&
  getState() const noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline stateType&
  getState() noexcept = 0;
};

template<typename TStatefulTraits>
class [[maybe_unused]] AnyStatefulBase :
    public virtual TStatefulTraits::abstractType
{
  using stateType [[maybe_unused]] =
      typename TStatefulTraits::stateType;

  static_assert(
      isAnyState(meta::type_c<stateType>),
      "AnyStatefulBase requires AnyState.");

  using abstractType [[maybe_unused]] =
      typename TStatefulTraits::abstractType;

  static_assert(
      isAnyStateful(meta::type_c<abstractType>),
      "AnyStatefulBase requires AnyStateful.");


 public:
  [[maybe_unused]] inline explicit AnyStatefulBase(stateType state)
      : state(move(state))
  {
  }


  [[maybe_unused, nodiscard]] inline const stateType&
  getState() const noexcept final
  {
    return state;
  }

  [[maybe_unused, nodiscard]] inline stateType&
  getState() noexcept final
  {
    return state;
  }


 protected:
  [[maybe_unused]] stateType state;
};

template<typename TStatefulTraits>
class JuceStatefulBase :
    public AnyStatefulBase<TStatefulTraits>,
    protected StateListener
{
  using stateType [[maybe_unused]] =
      typename TStatefulTraits::stateType;

  static_assert(
      isJuceState(meta::type_c<stateType>),
      "StatefulBase requires JuceState.");


 public:
  [[maybe_unused]] explicit JuceStatefulBase(stateType state)
      : AnyStatefulBase<AnyStatefulTraits<stateType>>(move(state))
  {
    BLOOPER_ASSERT(this->getState().isValid());

    this->getState().addListener(this);
  }

  [[maybe_unused]] ~JuceStatefulBase() override
  {
    this->getState().removeListener(this);
  }


 protected:
  void valueTreeChanged() override { }
};


using StatefulTraits = AnyStatefulTraits<State>;

using State = StatefulTraits::stateType;
using AbstractStateful = StatefulTraits::abstractType;
using StatefulBase = StatefulTraits::baseType;

BLOOPER_STATIC_ASSERT(
    isAnyStatefulTraits(meta::type_c<StatefulTraits>),
    "StatefulTraits must satisfy StatefulTraits.");

BLOOPER_STATIC_ASSERT(
    isAnyState(meta::type_c<State>),
    "State must satisfy State.");

BLOOPER_STATIC_ASSERT(
    isStateful(meta::type_c<AbstractStateful>),
    "AbstractStateful must satisfy Stateful.");

BLOOPER_STATIC_ASSERT(
    isStatefulBase(meta::type_c<StatefulBase>),
    "StatefulBase must satisfy StatefulBase.");


// Explicit instantiation

extern template class AnyAbstractStateful<StatefulTraits>;

extern template class AnyStatefulBase<StatefulTraits>;

BLOOPER_NAMESPACE_END

#endif // BLOOPER_STATEFUL_HPP
