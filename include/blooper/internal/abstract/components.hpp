#ifndef BLOOPER_ABSTRACT_COMPONENTS_HPP
#define BLOOPER_ABSTRACT_COMPONENTS_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>

#include <blooper/internal/abstract/prelude.hpp>
#include <blooper/internal/abstract/meta.hpp>
#include <blooper/internal/abstract/traits.hpp>
#include <blooper/internal/abstract/juceTraits.hpp>

#include <blooper/internal/abstract/stateful.hpp>
#include <blooper/internal/abstract/contextual.hpp>

BLOOPER_NAMESPACE_BEGIN

[[maybe_unused]] inline constexpr auto isAnyComponent =
    meta::satisfies_all(
        isJuceComponent,
        isAnyContextual,
        isAnyStateful);

template<typename TComponentTraits>
[[maybe_unused]] inline constexpr auto isAnyComponentBase =
    meta::satisfies_all(
        meta::reverse_partial(
            meta::traits::is_constructible,
            meta::type_c<typename TComponentTraits::
                             contextualTraits::contextType&>,
            meta::type_c<typename TComponentTraits::
                             statefulTraits::stateType>),
        isAnyComponent);

[[maybe_unused]] inline constexpr auto isCoreComponent =
    meta::satisfies_all(
        isAnyComponent,
        isCoreContextual,
        isStateful);

[[maybe_unused]] inline constexpr auto isComponent =
    meta::satisfies_all(
        isAnyComponent,
        isContextual,
        isStateful);

[[maybe_unused]] inline constexpr auto isCoreComponentBase =
    meta::satisfies_all(
        meta::reverse_partial(
            meta::traits::is_constructible,
            meta::type_c<AbstractCoreContext&>,
            meta::type_c<State>),
        isCoreComponent);

[[maybe_unused]] inline constexpr auto isComponentBase =
    meta::satisfies_all(
        meta::reverse_partial(
            meta::traits::is_constructible,
            meta::type_c<AbstractContext&>,
            meta::type_c<State>),
        isComponent);


[[maybe_unused]] inline constexpr auto isAnyComponentTraits =
    meta::attribute(
        [](auto&& toCheck)
            -> decltype(meta::and_(
                isAnyContextualTraits(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     contextualTraits>),
                isAnyStatefulTraits(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     statefulTraits>),
                isJuceComponent(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     juceBaseType>),

                isAnyComponent(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     abstractType>),
                isAnyComponentBase<std::decay_t<decltype(toCheck)>>(
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

template<typename TComponentTraits>
class AnyAbstractComponent;

template<typename TComponentTraits>
class AnyComponentBase;

template<
    typename TContextualTraits,
    typename TStatefulTraits,
    typename TJuceBase = JuceComponent,
    typename = void>
struct AnyComponentTraits
{
  static_assert(
      isAnyContextualTraits(meta::type_c<TContextualTraits>),
      "AnyComponentTraits requires AnyContext.");

  static_assert(
      isAnyStatefulTraits(meta::type_c<TStatefulTraits>),
      "AnyComponentTraits requires AnyState.");

  static_assert(
      isJuceComponent(meta::type_c<TJuceBase>),
      "AnyComponentTraits requires a JuceComponent.");


  using contextualTraits [[maybe_unused]] = TContextualTraits;
  using statefulTraits [[maybe_unused]] = TStatefulTraits;

  using juceBaseType [[maybe_unused]] = TJuceBase;


  using abstractType [[maybe_unused]] =
      AnyAbstractComponent<AnyComponentTraits>;

  using baseType [[maybe_unused]] =
      AnyComponentBase<AnyComponentTraits>;
};


template<typename TComponentTraits>
class [[maybe_unused]] AnyAbstractComponent :
    public virtual TComponentTraits::contextualTraits::abstractType,
    public virtual TComponentTraits::statefulTraits::abstractType,
    public virtual TComponentTraits::juceBaseType
{
  using contextType [[maybe_unused]] =
      typename TComponentTraits::contextualTraits::contextType;

  static_assert(
      isAnyContext(meta::type_c<contextType>),
      "AnyComponentBase requires AnyContext.");

  using abstractContextualType [[maybe_unused]] =
      typename TComponentTraits::contextualTraits::abstractType;

  static_assert(
      isAnyContextual(meta::type_c<abstractContextualType>),
      "AnyAbstractComponent requires a Contextual.");


  using stateType [[maybe_unused]] =
      typename TComponentTraits::statefulTraits::stateType;

  static_assert(
      isAnyState(meta::type_c<stateType>),
      "AnyComponentBase requires AnyState.");

  using abstractStatefulType [[maybe_unused]] =
      typename TComponentTraits::statefulTraits::abstractType;

  static_assert(
      isStateful(meta::type_c<abstractStatefulType>),
      "AnyAbstractComponent requires a Stateful.");


  using juceBaseType [[maybe_unused]] =
      typename TComponentTraits::juceBaseType;

  static_assert(
      isJuceComponent(meta::type_c<juceBaseType>),
      "AnyAbstractComponent requires a JuceComponent.");


 public:
  [[maybe_unused]] inline AnyAbstractComponent() = default;

  [[maybe_unused]] inline ~AnyAbstractComponent() override = default;
};

template<typename TComponentTraits>
class [[maybe_unused]] AnyComponentBase :
    public virtual TComponentTraits::abstractType,
    public TComponentTraits::contextualTraits::baseType,
    public TComponentTraits::statefulTraits::baseType
{
  using abstractType [[maybe_unused]] =
      typename TComponentTraits::abstractType;

  static_assert(
      isAnyComponent(meta::type_c<abstractType>),
      "AnyComponentBase requires AnyComponent.");


  using contextType [[maybe_unused]] =
      typename TComponentTraits::contextualTraits::contextType;

  static_assert(
      isAnyContext(meta::type_c<contextType>),
      "AnyComponentBase requires AnyContext.");

  using contextualBaseType [[maybe_unused]] =
      typename TComponentTraits::contextualTraits::baseType;

  static_assert(
      isAnyContextualBase<typename TComponentTraits::contextualTraits>(
          meta::type_c<contextualBaseType>),
      "AnyComponentBase requires AnyContextualBase.");


  using stateType [[maybe_unused]] =
      typename TComponentTraits::statefulTraits::stateType;

  static_assert(
      isAnyState(meta::type_c<stateType>),
      "AnyComponentBase requires AnyState.");

  using statefulBaseType [[maybe_unused]] =
      typename TComponentTraits::statefulTraits::baseType;

  static_assert(
      isAnyStatefulBase<typename TComponentTraits::statefulTraits>(
          meta::type_c<statefulBaseType>),
      "AnyComponentBase requires AnyStatefulBase.");


 public:
  [[maybe_unused]] inline explicit AnyComponentBase(
      contextType& context,
      stateType    state)
      : contextualBaseType(context),
        statefulBaseType(move(state))
  {
    if constexpr (
        decltype(isCoreContext(
            meta::typeid_(this->getContext()))){})
    {
      this->setLookAndFeel(
          std::addressof(
              this->getContext().getLookAndFeel()));
    }
  }
};


using CoreComponentTraits =
    AnyComponentTraits<
        CoreContextualTraits,
        StatefulTraits,
        JuceComponent>;

using AbstractCoreComponent = CoreComponentTraits::abstractType;
using CoreComponentBase = CoreComponentTraits::baseType;

BLOOPER_STATIC_ASSERT(
    isAnyComponentTraits(meta::type_c<CoreComponentTraits>),
    "CoreComponentTraits must satisfy AnyComponentTraits.");

BLOOPER_STATIC_ASSERT(
    isCoreComponent(meta::type_c<AbstractCoreComponent>),
    "AbstractCoreComponent must satisfy CoreComponent.");

BLOOPER_STATIC_ASSERT(
    isCoreComponentBase(meta::type_c<CoreComponentBase>),
    "CoreComponent must satisfy CoreComponentBase.");

using ComponentTraits =
    AnyComponentTraits<
        ContextualTraits,
        StatefulTraits,
        JuceComponent>;

using AbstractComponent = ComponentTraits::abstractType;
using ComponentBase = ComponentTraits::baseType;

BLOOPER_STATIC_ASSERT(
    isAnyComponentTraits(meta::type_c<ComponentTraits>),
    "ComponentTraits must satisfy AnyComponentTraits.");

BLOOPER_STATIC_ASSERT(
    isComponent(meta::type_c<AbstractComponent>),
    "AbstractComponent must satisfy Component.");

BLOOPER_STATIC_ASSERT(
    isComponentBase(meta::type_c<ComponentBase>),
    "Component must satisfy ComponentBase.");


// Children

template<
    typename TChild,
    typename TParent,
    typename... TArgs>
[[maybe_unused, nodiscard]] inline auto
makeJuceChild(
    TParent& parent,
    TArgs&&... args)
{
  static_assert(
      meta::and_(
          isAnyComponent(BLOOPER_TYPEID(parent)),
          isJuceComponent(meta::type_c<TChild>)),
      "makeJuceChild requires a parent component and "
      "a child component");

  return std::make_unique<TChild>(
      BLOOPER_FORWARD(args)...);
}

template<
    typename TChild,
    typename TParent,
    typename... TArgs>
[[maybe_unused, nodiscard]] inline auto
makeChild(
    TParent& parent,
    TArgs&&... args)
    -> decltype(makeJuceChild<TChild>(
        parent,
        parent.getContext(),
        parent.getState()
            .getOrCreateChildWithName(
                TChild::stateId,
                nullptr),
        BLOOPER_FORWARD(args)...))
{
  static_assert(
      isAnyComponent(meta::type_c<TChild>),
      "makeChild requires a parent component and "
      "a child component");

  return makeJuceChild<TChild>(
      parent,
      parent.getContext(),
      parent.getState()
          .getOrCreateChildWithName(
              TChild::stateId,
              nullptr),
      BLOOPER_FORWARD(args)...);
}

template<
    typename TChild,
    typename TOptions,
    typename TParent,
    typename... TArgs>
[[maybe_unused, nodiscard]] inline auto
makeChild(
    TParent&   parent,
    TOptions&& options,
    TArgs&&... args)
    -> decltype(makeChild<TChild>(
        parent,
        BLOOPER_FORWARD(args)...,
        BLOOPER_FORWARD(options)))
{
  return makeChild<TChild>(
      parent,
      BLOOPER_FORWARD(args)...,
      BLOOPER_FORWARD(options));
}

template<
    typename TChild,
    typename TParent,
    typename... TArgs>
[[maybe_unused, nodiscard]] inline auto
addJuceChild(
    TParent& parent,
    TArgs&&... args)
{
  auto child =
      makeJuceChild<TChild>(
          parent,
          BLOOPER_FORWARD(args)...);
  parent.addAndMakeVisible(*child);

  return std::move(child);
}

template<
    typename TChild,
    typename TParent,
    typename... TArgs>
[[maybe_unused, nodiscard]] inline auto
addChild(
    TParent& parent,
    TArgs&&... args)
    -> decltype(makeChild<TChild>(
        parent,
        BLOOPER_FORWARD(args)...))
{
  auto child =
      makeChild<TChild>(
          parent,
          BLOOPER_FORWARD(args)...);
  parent.addAndMakeVisible(*child);

  return std::move(child);
}

template<
    typename TChild,
    typename TOptions,
    typename TParent,
    typename... TArgs>
[[maybe_unused, nodiscard]] inline auto
addChild(
    TParent&   parent,
    TOptions&& options,
    TArgs&&... args)
    -> decltype(makeChild<TChild>(
        parent,
        BLOOPER_FORWARD(args)...,
        BLOOPER_FORWARD(options)))
{
  return addChild<TChild>(
      parent,
      BLOOPER_FORWARD(args)...,
      BLOOPER_FORWARD(options));
}


// Explicit instantiation

extern template class AnyAbstractComponent<CoreComponentTraits>;
extern template class AnyAbstractComponent<ComponentTraits>;

extern template class AnyComponentBase<CoreComponentTraits>;
extern template class AnyComponentBase<ComponentTraits>;

BLOOPER_NAMESPACE_END

#endif // BLOOPER_ABSTRACT_COMPONENTS_HPP
