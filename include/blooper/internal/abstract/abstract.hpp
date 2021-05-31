#ifndef BLOOPER_ABSTRACT_HPP
#define BLOOPER_ABSTRACT_HPP


#include <blooper/internal/macros/macros.hpp>

#include <blooper/internal/abstract/id.hpp>

#include <blooper/internal/abstract/meta.hpp>
#include <blooper/internal/abstract/traits.hpp>

#include <blooper/internal/abstract/juceTraits.hpp>


// TODO:
//  concepts (Core)?(Contextual|Component|Window),
//  state/listener/identifier abstractions,
//  base construction assumptions,
//  UndoManager management,
//  fix template base tests


BLOOPER_NAMESPACE_BEGIN

// Checks

[[maybe_unused]] inline constexpr auto isInvokeable =
    meta::check(
        [](auto&& toCheck, auto&&... args)
            -> decltype(toCheck(
                std::forward<decltype(args)>(args)...)) {});

[[maybe_unused]] inline constexpr auto isCallback =
    meta::check(
        [](auto&& toCheck)
            -> decltype(toCheck()) {});


// State

[[maybe_unused]] inline constexpr auto isAnyState =
    meta::always(meta::true_c);

using State [[maybe_unused]] = JuceState;
using StateListener [[maybe_unused]] = JuceStateListener;
using StateIdentifier [[maybe_unused]] = JuceStateIdentifier;


[[maybe_unused]] inline constexpr auto isAnyStateful =
    meta::attribute(
        [](auto&& toCheck)
            -> decltype(isAnyState(meta::typeid_(toCheck.getState()))) {}) ^
    meta::inherit ^
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
    meta::inherit ^ isAnyStateful;

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
    meta::inherit ^
    meta::check(
        [](auto&& toCheck)
            -> decltype(meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         stateType>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         abstractType>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         baseType>) {});

template<typename TStatefulTraits>
class AbstractAnyStateful;

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

  using abstractType = AbstractAnyStateful<AnyStatefulTraits>;
  using baseType = AnyStatefulBase<AnyStatefulTraits>;
};

template<typename TState>
struct AnyStatefulTraits<
    TState,
    std::enable_if_t<isJuceState(meta::type_c<TState>)>>
{
  using stateType = State;

  using abstractType = AbstractAnyStateful<AnyStatefulTraits>;
  using baseType = JuceStatefulBase<AnyStatefulTraits>;
};


template<typename TStatefulTraits>
class [[maybe_unused]] AbstractAnyStateful
{
  using stateType [[maybe_unused]] = typename TStatefulTraits::stateType;

  static_assert(
      isAnyState(meta::type_c<stateType>),
      "AbstractAnyStateful requires AnyState.");


 public:
  [[maybe_unused]] inline AbstractAnyStateful() = default;

  [[maybe_unused]] virtual inline ~AbstractAnyStateful() = default;


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


// Context

[[maybe_unused]] inline constexpr auto isAnyContext =
    meta::always(meta::true_c);

[[maybe_unused]] inline constexpr auto isCoreContext =
    meta::satisfies_all(
        isAnyStateful,
        isAnyContext,
        meta::attribute(
            [](auto&& toCheck)
                -> decltype(meta::and_(
                    meta::traits::is_same(
                        meta::typeid_(toCheck.getEngine()),
                        meta::type_c<JuceEngine>),
                    meta::traits::is_same(
                        meta::typeid_(toCheck.getLookAndFeel()),
                        meta::type_c<JuceLookAndFeel>),
                    meta::traits::is_same(
                        meta::typeid_(toCheck.getUndoManager()),
                        meta::type_c<JuceUndoManager>))) {}) ^
            meta::inherit ^
            meta::check(
                [](auto&& toCheck)
                    -> decltype(toCheck.getEngine(),
                                toCheck.getLookAndFeel(),
                                toCheck.getUndoManager()) {}));

[[maybe_unused]] inline constexpr auto isContext =
    meta::satisfies_all(
        isCoreContext,
        meta::attribute(
            [](auto&& toCheck)
                -> decltype(meta::and_(
                    meta::traits::is_same(
                        meta::typeid_(toCheck.getProject()),
                        meta::type_c<JuceProject>),
                    meta::traits::is_same(
                        meta::typeid_(toCheck.getEdit()),
                        meta::type_c<JuceEdit>),
                    meta::traits::is_same(
                        meta::typeid_(toCheck.getTransport()),
                        meta::type_c<JuceTransport>))) {}) ^
            meta::inherit ^
            meta::check(
                [](auto&& toCheck)
                    -> decltype(toCheck.getProject(),
                                toCheck.getEdit(),
                                toCheck.getTransport()) {}));

template<typename TStatefulTraits>
class [[maybe_unused]] AbstractAnyCoreContext :
    public virtual TStatefulTraits::abstractType
{
  using stateType [[maybe_unused]] =
      typename TStatefulTraits::stateType;

  static_assert(
      isAnyState(meta::type_c<stateType>),
      "AbstractAnyCoreContext requires AnyState.");

  using abstractStateful [[maybe_unused]] =
      typename TStatefulTraits::abstractType;

  static_assert(
      isAnyStateful(meta::type_c<abstractStateful>),
      "AbstractAnyCoreContext requires AnyStateful.");


 public:
  [[maybe_unused]] inline AbstractAnyCoreContext() = default;

  [[maybe_unused]] inline ~AbstractAnyCoreContext() override = default;


  [[maybe_unused, nodiscard]] virtual inline const JuceEngine&
  getEngine() const noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline JuceEngine&
  getEngine() noexcept = 0;


  [[maybe_unused, nodiscard]] virtual inline const JuceLookAndFeel&
  getLookAndFeel() const noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline JuceLookAndFeel&
  getLookAndFeel() noexcept = 0;


  [[maybe_unused, nodiscard]] virtual inline const JuceUndoManager&
  getUndoManager() const noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline JuceUndoManager&
  getUndoManager() noexcept = 0;
};

template<typename TStatefulTraits>
class [[maybe_unused]] AbstractAnyContext :
    public virtual AbstractAnyCoreContext<TStatefulTraits>
{
 public:
  [[maybe_unused]] inline AbstractAnyContext() = default;

  [[maybe_unused]] inline ~AbstractAnyContext() override = default;


  [[maybe_unused, nodiscard]] virtual inline const JuceProject&
  getProject() const noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline JuceProject&
  getProject() noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline JuceProjectConstRef
  getProjectRef() const noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline JuceProjectRef
  getProjectRef() noexcept = 0;


  [[maybe_unused, nodiscard]] virtual inline const JuceEdit&
  getEdit() const noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline JuceEdit&
  getEdit() noexcept = 0;


  [[maybe_unused, nodiscard]] virtual inline const JuceTransport&
  getTransport() const noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline JuceTransport&
  getTransport() noexcept = 0;
};


[[maybe_unused]] constexpr auto isAnyContextual =
    meta::check([](auto&& toCheck) -> decltype(toCheck.getContext()) {});

template<typename TContextualTraits>
[[maybe_unused]] inline constexpr auto isAnyContextualBase =
    meta::satisfies_all(
        meta::reverse_partial(
            meta::traits::is_constructible,
            meta::type_c<typename TContextualTraits::contextType&>),
        isAnyContextual);

[[maybe_unused]] inline constexpr auto isCoreContextual =
    meta::attribute(
        [](auto&& toCheck)
            -> decltype(isCoreContext(
                meta::typeid_(toCheck.getContext()))) {}) ^
    meta::inherit ^ isAnyContextual;

[[maybe_unused]] inline constexpr auto isContextual =
    meta::attribute(
        [](auto&& toCheck)
            -> decltype(isContext(
                meta::typeid_(toCheck.getContext()))) {}) ^
    meta::inherit ^ isAnyContextual;

[[maybe_unused]] inline constexpr auto isCoreContextualBase =
    meta::satisfies_all(
        meta::reverse_partial(
            meta::traits::is_constructible,
            meta::type_c<AbstractAnyCoreContext<StatefulTraits>&>),
        isCoreContextual);

[[maybe_unused]] inline constexpr auto isContextualBase =
    meta::satisfies_all(
        meta::reverse_partial(
            meta::traits::is_constructible,
            meta::type_c<AbstractAnyContext<StatefulTraits>&>),
        isContextual);


[[maybe_unused]] inline constexpr auto isAnyContextualTraits =
    meta::attribute(
        [](auto&& toCheck)
            -> decltype(meta::and_(
                isAnyContext(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     contextType>),
                isAnyContextual(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     abstractType>),
                isAnyContextualBase<std::decay_t<decltype(toCheck)>>(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     baseType>))) {}) ^
    meta::inherit ^
    meta::check(
        [](auto&& toCheck)
            -> decltype(meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         contextType>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         abstractType>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         baseType>) {});

template<typename TContextualTraits>
class AbstractAnyContextual;

template<typename TContextualTraits>
class AnyContextualBase;

template<typename TContext, typename = void>
struct AnyContextualTraits
{
  static_assert(
      isAnyContext(meta::type_c<TContext>),
      "AnyContextualTraits requires AnyContext.");


  using contextType = TContext;

  using abstractType = AbstractAnyContextual<AnyContextualTraits>;
  using baseType = AnyContextualBase<AnyContextualTraits>;
};


template<typename TContextualTraits>
class [[maybe_unused]] AbstractAnyContextual
{
  using contextType [[maybe_unused]] =
      typename TContextualTraits::contextType;

  static_assert(
      isAnyContext(meta::type_c<contextType>),
      "AbstractAnyContextual requires AnyContext.");


 public:
  [[maybe_unused]] inline AbstractAnyContextual() = default;

  [[maybe_unused]] virtual inline ~AbstractAnyContextual() = default;


  [[maybe_unused, nodiscard]] virtual inline const contextType&
  getContext() const noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline contextType&
  getContext() noexcept = 0;
};

template<typename TContextualTraits>
class [[maybe_unused]] AnyContextualBase :
    public virtual TContextualTraits::abstractType
{
  using contextType [[maybe_unused]] =
      typename TContextualTraits::contextType;

  static_assert(
      isAnyContext(meta::type_c<contextType>),
      "AnyContextualBase requires AnyContext.");

  using abstractType [[maybe_unused]] =
      typename TContextualTraits::abstractType;

  static_assert(
      isAnyContextual(meta::type_c<abstractType>),
      "AnyContextualBase requires AnyContextual.");


 public:
  [[maybe_unused]] inline explicit AnyContextualBase(contextType& context)
      : context(context)
  {
  }


  [[maybe_unused, nodiscard]] inline const contextType&
  getContext() const noexcept final
  {
    return context;
  }

  [[maybe_unused, nodiscard]] inline contextType&
  getContext() noexcept final
  {
    return context;
  }


 protected:
  [[maybe_unused]] contextType& context;
};


using CoreContextualTraits =
    AnyContextualTraits<
        AbstractAnyCoreContext<
            StatefulTraits>>;

using AbstractCoreContext = CoreContextualTraits::contextType;
using AbstractCoreContextual = CoreContextualTraits::abstractType;
using CoreContextualBase = CoreContextualTraits::baseType;

BLOOPER_STATIC_ASSERT(
    isAnyContextualTraits(meta::type_c<CoreContextualTraits>),
    "CoreContextualTraits must satisfy ContextualTraits.");

BLOOPER_STATIC_ASSERT(
    isCoreContext(meta::type_c<AbstractCoreContext>),
    "AbstractCoreContext must satisfy CoreContext.");

BLOOPER_STATIC_ASSERT(
    isCoreContextual(meta::type_c<AbstractCoreContextual>),
    "AbstractCoreContextual must satisfy CoreContextual.");

BLOOPER_STATIC_ASSERT(
    isCoreContextualBase(meta::type_c<CoreContextualBase>),
    "CoreContextualBase must satisfy CoreContextualBase.");

using ContextualTraits =
    AnyContextualTraits<
        AbstractAnyContext<
            StatefulTraits>>;

using AbstractContext = ContextualTraits::contextType;
using AbstractContextual = ContextualTraits::baseType;
using ContextualBase = ContextualTraits::baseType;

BLOOPER_STATIC_ASSERT(
    isAnyContextualTraits(meta::type_c<ContextualTraits>),
    "ContextualTraits must satisfy ContextualTraits.");

BLOOPER_STATIC_ASSERT(
    isContext(meta::type_c<AbstractContext>),
    "AbstractContext must satisfy Context.");

BLOOPER_STATIC_ASSERT(
    isContextual(meta::type_c<AbstractContextual>),
    "AbstractContextual must satisfy Contextual.");

BLOOPER_STATIC_ASSERT(
    isContextualBase(meta::type_c<ContextualBase>),
    "ContextualBase must satisfy ContextualBase.");


// Component

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
    meta::inherit ^
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
    public TComponentTraits::juceBaseType
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
    this->setLookAndFeel(&context.getLookAndFeel());
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


// Window

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
    meta::inherit ^
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


    if constexpr (decltype(isCoreContext(meta::typeid_(this->getContext()))){})
    {
      auto& lookAndFeel = this->getContext().getLookAndFeel();

      this->setLookAndFeel(
          std::addressof(lookAndFeel));

      this->setBackgroundColour(
          lookAndFeel.findColour(
              juce::ResizableWindow::backgroundColourId));
    }
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


// plugins

[[maybe_unused]] inline constexpr auto isAnyPlugin =
    meta::satisfies_all(
        isJucePlugin);

[[maybe_unused]] inline constexpr auto isAnyPluginRef =
    meta::satisfies_all(
        isJucePluginRef);

[[maybe_unused]] inline constexpr auto isAnyPluginConstRef =
    meta::satisfies_all(
        isJucePluginConstRef);


[[maybe_unused]] inline constexpr auto isAnyPluginTraits =
    meta::attribute(
        [](auto&& toCheck)
            -> decltype(meta::and_(
                isAnyPlugin(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     pluginType>),
                isAnyPluginRef(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     pluginRefType>),
                isAnyPluginConstRef(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     pluginConstRefType>))) {}) ^
    meta::inherit ^
    meta::check(
        [](auto&& toCheck)
            -> decltype(meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         pluginType>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         pluginRefType>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         pluginConstRefType>) {});

template<typename TPlugin>
struct [[maybe_unused]] AnyPluginTraits
{
  using pluginType [[maybe_unused]] = TPlugin;

  using pluginRefType [[maybe_unused]] =
      juce::ReferenceCountedObjectPtr<pluginType>;

  // can't put const chere because of how juce does reference counting...
  using pluginConstRefType [[maybe_unused]] =
      juce::ReferenceCountedObjectPtr<pluginType>;
};


[[maybe_unused]] inline constexpr auto isAnyPluginContentComponent =
    meta::satisfies_all(
        isAnyComponent,
        meta::attribute(
            [](auto&& toCheck)
                -> decltype(meta::and_(
                    isAnyPlugin(meta::typeid_(toCheck.getPlugin())),
                    isAnyPluginRef(meta::typeid_(toCheck.getPluginRef())),

                    meta::traits::is_same(
                        meta::typeid_(toCheck.getConstrainer()),
                        meta::type_c<JuceConstrainer*>),
                    meta::traits::is_same(
                        meta::typeid_(toCheck.checkIsResizable()),
                        meta::type_c<bool>))) {}) ^

            meta::inherit ^
            meta::check(
                [](auto&& toCheck)
                    -> decltype(toCheck.getPlugin(),
                                toCheck.getPluginRef(),

                                toCheck.getConstrainer(),
                                toCheck.checkIsResizable(),

                                toCheck.recreate()) {}));

template<
    typename TPluginContentComponentTraits,
    typename THeldPluginTraits =
        typename TPluginContentComponentTraits::pluginTraits>
[[maybe_unused]] inline constexpr auto isAnyPluginContentComponentBase =
    meta::satisfies_all(
        meta::reverse_partial(
            meta::traits::is_constructible,
            meta::type_c<typename THeldPluginTraits::pluginRefType>,
            meta::type_c<typename TPluginContentComponentTraits::
                             componentTraits::contextualTraits::contextType&>,
            meta::type_c<typename TPluginContentComponentTraits::
                             componentTraits::statefulTraits::stateType>),
        isAnyPluginContentComponent);


[[maybe_unused]] inline constexpr auto isAnyPluginContentTraits =
    meta::attribute(
        [](auto&& toCheck)
            -> decltype(meta::and_(
                isAnyPluginTraits(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     pluginTraits>),
                isAnyComponentTraits(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     componentTraits>),
                isAnyPluginContentComponent(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     abstractType>),
                isAnyPluginContentComponentBase<
                    std::decay_t<decltype(toCheck)>>(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     baseType>))) {}) ^
    meta::inherit ^
    meta::check(
        [](auto&& toCheck)
            -> decltype(meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         pluginTraits>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         componentTraits>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         abstractType>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         baseType>) {});

template<typename TPluginContentTraits>
class [[maybe_unused]] AnyAbstractPluginContentComponent;

template<
    typename TPluginContentTraits,
    typename THeldPlugin = typename TPluginContentTraits::pluginTraits>
class [[maybe_unused]] AnyPluginContentComponentBase;

template<
    typename TPluginTraits,
    typename TComponentTraits,
    typename = void>
struct [[maybe_unused]] AnyPluginContentTraits
{
  using pluginTraits [[maybe_unused]] = TPluginTraits;

  static_assert(
      isAnyPluginTraits(meta::type_c<pluginTraits>),
      "AnyPluginContentTraits requires AnyPluginTraits.");

  using componentTraits [[maybe_unused]] = TComponentTraits;

  static_assert(
      isAnyComponentTraits(meta::type_c<componentTraits>),
      "AnyPluginContentTraits requires AnyComponentTraits.");


  using abstractType [[maybe_unused]] =
      AnyAbstractPluginContentComponent<
          AnyPluginContentTraits>;

  using baseType [[maybe_unused]] =
      AnyPluginContentComponentBase<
          AnyPluginContentTraits>;
};


[[maybe_unused]] inline constexpr auto isAnyHeldPluginContentTraits =
    meta::attribute(
        [](auto&& toCheck)
            -> decltype(meta::and_(
                isAnyPluginContentTraits(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     contentTraits>),
                isAnyPluginTraits(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     heldPluginTraits>),
                isAnyPluginContentComponentBase<
                    typename std::decay_t<decltype(toCheck)>::
                        contentTraits,
                    typename std::decay_t<decltype(toCheck)>::
                        heldPluginTraits>(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     baseType>))) {}) ^
    meta::inherit ^
    meta::check(
        [](auto&& toCheck)
            -> decltype(meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         contentTraits>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         heldPluginTraits>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         baseType>) {});

template<
    typename TPluginContentTraits,
    typename THeldPluginTraits,
    typename = void>
struct [[maybe_unused]] AnyHeldPluginContentTraits
{
  using contentTraits = TPluginContentTraits;

  static_assert(
      isAnyPluginContentTraits(meta::type_c<contentTraits>),
      "AnyHeldPluginContentTraits requires AnyPluginContentTraits.");

  using heldPluginTraits [[maybe_unused]] = THeldPluginTraits;

  static_assert(
      isAnyPluginTraits(meta::type_c<heldPluginTraits>),
      "AnyHeldPluginContentTraits requires AnyPluginTraits.");


  using baseType [[maybe_unused]] =
      AnyPluginContentComponentBase<
          contentTraits,
          heldPluginTraits>;
};


template<typename TPluginContentTraits>
class [[maybe_unused]] AnyAbstractPluginContentComponent :
    public virtual TPluginContentTraits::componentTraits::abstractType
{
  using abstractComponentType [[maybe_unused]] =
      typename TPluginContentTraits::componentTraits::abstractType;

  static_assert(
      isAnyComponent(meta::type_c<abstractComponentType>),
      "AnyAbstractPluginContentComponent requires AnyComponent.");


  using pluginType [[maybe_unused]] =
      typename TPluginContentTraits::pluginTraits::pluginType;

  static_assert(
      isAnyPlugin(meta::type_c<pluginType>),
      "AnyAbstractPluginContentComponent requires AnyPlugin.");

  using pluginRefType [[maybe_unused]] =
      typename TPluginContentTraits::pluginTraits::pluginRefType;

  static_assert(
      isAnyPluginRef(meta::type_c<pluginRefType>),
      "AnyAbstractPluginContentComponent requires AnyPluginRef.");

  using pluginConstRefType [[maybe_unused]] =
      typename TPluginContentTraits::pluginTraits::pluginConstRefType;

  static_assert(
      isAnyPluginConstRef(meta::type_c<pluginConstRefType>),
      "AnyAbstractPluginContentComponent requires AnyPluginConstRef.");


 public:
  [[maybe_unused]] inline AnyAbstractPluginContentComponent() = default;

  [[maybe_unused]] inline ~AnyAbstractPluginContentComponent() override = default;


  [[maybe_unused, nodiscard]] inline virtual const pluginType&
  getPlugin() const noexcept = 0;

  [[maybe_unused, nodiscard]] inline virtual pluginType&
  getPlugin() noexcept = 0;


  [[maybe_unused, nodiscard]] inline virtual pluginConstRefType
  getPluginRef() const noexcept = 0;

  [[maybe_unused, nodiscard]] inline virtual pluginRefType
  getPluginRef() noexcept = 0;


  [[maybe_unused, nodiscard]] inline virtual const JuceConstrainer*
  getConstrainer() const noexcept = 0;

  [[maybe_unused, nodiscard]] inline virtual JuceConstrainer*
  getConstrainer() noexcept = 0;


  [[maybe_unused, nodiscard]] inline virtual bool
  checkIsResizable() const noexcept = 0;


  [[maybe_unused]] virtual void recreate() = 0;
};

template<typename TPluginContentTraits, typename THeldPluginTraits>
class [[maybe_unused]] AnyPluginContentComponentBase :
    public virtual TPluginContentTraits::abstractType,
    public TPluginContentTraits::componentTraits::baseType
{
  using abstractType [[maybe_unused]] =
      typename TPluginContentTraits::abstractType;


  using componentBaseType [[maybe_unused]] =
      typename TPluginContentTraits::componentTraits::baseType;

  static_assert(
      isAnyComponentBase<typename TPluginContentTraits::componentTraits>(
          meta::type_c<componentBaseType>),
      "AnyPluginContentComponentBase requires AnyComponentBase.");


  using contextType [[maybe_unused]] =
      typename TPluginContentTraits::
          componentTraits::
              contextualTraits::
                  contextType;

  static_assert(
      isAnyContext(meta::type_c<contextType>),
      "AnyPluginContentComponentBase requires AnyContext.");

  using stateType [[maybe_unused]] =
      typename TPluginContentTraits::
          componentTraits::
              statefulTraits::
                  stateType;

  static_assert(
      isAnyState(meta::type_c<stateType>),
      "AnyPluginContentComponentBase requires AnyState.");


  using pluginType [[maybe_unused]] =
      typename TPluginContentTraits::pluginTraits::pluginType;

  static_assert(
      isAnyPlugin(meta::type_c<pluginType>),
      "AnyPluginContentComponentBase requires AnyPlugin.");

  using pluginRefType [[maybe_unused]] =
      typename TPluginContentTraits::pluginTraits::pluginRefType;

  static_assert(
      isAnyPluginRef(meta::type_c<pluginRefType>),
      "AnyPluginContentComponentBase requires AnyPluginRef.");

  using pluginConstRefType [[maybe_unused]] =
      typename TPluginContentTraits::pluginTraits::pluginConstRefType;

  static_assert(
      isAnyPluginConstRef(meta::type_c<pluginConstRefType>),
      "AnyPluginContentComponentBase requires AnyPluginConstRef.");


  using heldPluginType [[maybe_unused]] =
      typename THeldPluginTraits::pluginType;

  static_assert(
      meta::and_(
          isAnyPlugin(meta::type_c<heldPluginType>),
          meta::traits::is_convertible(
              meta::type_c<heldPluginType&>,
              meta::type_c<pluginType&>)),
      "AnyPluginContentComponentBase requires AnyPlugin for "
      "the held plugin type that is convertible to pluginType.");

  using heldPluginRefType [[maybe_unused]] =
      typename THeldPluginTraits::pluginRefType;

  static_assert(
      meta::and_(
          isAnyPluginRef(meta::type_c<heldPluginRefType>),
          meta::traits::is_convertible(
              meta::type_c<heldPluginRefType>,
              meta::type_c<pluginRefType>)),
      "AnyPluginContentComponentBase requires AnyPluginRef for "
      "held plugin ref type that is convertible to the pluginRefType.");

  using heldPluginConstRefType [[maybe_unused]] =
      typename THeldPluginTraits::pluginConstRefType;

  static_assert(
      meta::and_(
          isAnyPluginConstRef(meta::type_c<heldPluginConstRefType>),
          meta::traits::is_convertible(
              meta::type_c<heldPluginConstRefType>,
              meta::type_c<pluginConstRefType>)),
      "AnyPluginContentComponentBase requires AnyPluginConstRef for "
      "held plugin const ref type that is convertible to the pluginConstRefType.");


 public:
  [[maybe_unused]] explicit AnyPluginContentComponentBase(
      heldPluginRefType plugin,
      contextType&      context,
      stateType         state)
      : componentBaseType(
            context,
            move(state)),
        plugin(move(plugin))
  {
    BLOOPER_ASSERT(this->plugin);
  }


  [[maybe_unused, nodiscard]] inline const pluginType&
  getPlugin() const noexcept final
  {
    return this->getHeldPlugin();
  }

  [[maybe_unused, nodiscard]] inline pluginType&
  getPlugin() noexcept final
  {
    return this->getHeldPlugin();
  }


  [[maybe_unused, nodiscard]] inline pluginConstRefType
  getPluginRef() const noexcept final
  {
    return this->getHeldPluginRef();
  }

  [[maybe_unused, nodiscard]] inline pluginRefType
  getPluginRef() noexcept final
  {
    return this->getHeldPluginRef();
  }


  [[maybe_unused, nodiscard]] inline const JuceConstrainer*
  getConstrainer() const noexcept override
  {
    return nullptr;
  }

  [[maybe_unused, nodiscard]] inline JuceConstrainer*
  getConstrainer() noexcept override
  {
    return nullptr;
  }


  [[maybe_unused, nodiscard]] inline bool
  checkIsResizable() const noexcept override
  {
    return true;
  }


  [[maybe_unused]] void recreate() override
  {
  }


 protected:
  [[maybe_unused, nodiscard]] inline const heldPluginType&
  getHeldPlugin() const noexcept
  {
    return *this->plugin;
  }

  [[maybe_unused, nodiscard]] inline heldPluginType&
  getHeldPlugin() noexcept
  {
    return *this->plugin;
  }

  [[maybe_unused, nodiscard]] inline heldPluginConstRefType
  getHeldPluginRef() const noexcept
  {
    return this->plugin;
  }

  [[maybe_unused, nodiscard]] inline heldPluginRefType
  getHeldPluginRef() noexcept
  {
    return this->plugin;
  }


 private:
  [[maybe_unused]] heldPluginRefType plugin;
};


using PluginTraits [[maybe_unused]] =
    AnyPluginTraits<JucePlugin>;

using Plugin [[maybe_unused]] =
    PluginTraits::pluginType;

using PluginRef [[maybe_unused]] =
    PluginTraits::pluginRefType;

using PluginConstRef [[maybe_unused]] =
    PluginTraits::pluginConstRefType;

BLOOPER_STATIC_ASSERT(
    isAnyPluginTraits(meta::type_c<PluginTraits>),
    "PluginTraits must satisfy AnyPluginTraits.");

BLOOPER_STATIC_ASSERT(
    isAnyPlugin(meta::type_c<Plugin>),
    "Plugin must satisfy AnyPlugin.");

BLOOPER_STATIC_ASSERT(
    isAnyPluginRef(meta::type_c<PluginRef>),
    "PluginRef must satisfy AnyPluginRef.");

BLOOPER_STATIC_ASSERT(
    isAnyPluginConstRef(meta::type_c<PluginConstRef>),
    "PluginConstRef must satisfy AnyPluginConstRef.");

using ExternalPluginTraits [[maybe_unused]] =
    AnyPluginTraits<JuceExternalPlugin>;

using ExternalPlugin [[maybe_unused]] =
    ExternalPluginTraits::pluginType;

using ExternalPluginRef [[maybe_unused]] =
    ExternalPluginTraits::pluginRefType;

using ExternalPluginConstRef [[maybe_unused]] =
    ExternalPluginTraits::pluginConstRefType;

BLOOPER_STATIC_ASSERT(
    isAnyPluginTraits(meta::type_c<ExternalPluginTraits>),
    "PluginTraits must satisfy AnyPluginTraits.");

BLOOPER_STATIC_ASSERT(
    isAnyPlugin(meta::type_c<ExternalPlugin>),
    "Plugin must satisfy AnyPlugin.");

BLOOPER_STATIC_ASSERT(
    isAnyPluginRef(meta::type_c<ExternalPluginRef>),
    "PluginRef must satisfy AnyPluginRef.");

BLOOPER_STATIC_ASSERT(
    isAnyPluginConstRef(meta::type_c<ExternalPluginConstRef>),
    "PluginConstRef must satisfy AnyPluginConstRef.");

using PluginContentTraits [[maybe_unused]] =
    AnyPluginContentTraits<
        PluginTraits,
        CoreComponentTraits>;

using AbstractPluginContentComponent [[maybe_unused]] =
    PluginContentTraits::abstractType;

using PluginContentComponentBase [[maybe_unused]] =
    PluginContentTraits::baseType;

using HeldExternalPluginContentTraits [[maybe_unused]] =
    AnyHeldPluginContentTraits<PluginContentTraits, ExternalPluginTraits>;

using ExternalPluginContentComponentBase [[maybe_unused]] =
    HeldExternalPluginContentTraits::baseType;

BLOOPER_STATIC_ASSERT(
    isAnyPluginContentTraits(
        meta::type_c<PluginContentTraits>),
    "PluginContentTraits must satisfy AnyPluginContentTraits.");

BLOOPER_STATIC_ASSERT(
    isAnyPluginContentComponent(
        meta::type_c<AbstractPluginContentComponent>),
    "AbstractPluginContentComponent must satisfy AnyPluginContentComponent.");

BLOOPER_STATIC_ASSERT(
    isAnyPluginContentComponentBase<PluginContentTraits>(
        meta::type_c<PluginContentComponentBase>),
    "PluginContentComponentBase must satisfy AnyPluginContentComponentBase.");

BLOOPER_STATIC_ASSERT(
    isAnyHeldPluginContentTraits(
        meta::type_c<HeldExternalPluginContentTraits>),
    "HeldExternalPluginContentTraits must satisfy AnyHeldPluginContentTraits.");

BLOOPER_STATIC_ASSERT(
    isAnyPluginContentComponentBase<PluginContentTraits>(
        meta::type_c<ExternalPluginContentComponentBase>),
    "ExternalPluginContentComponentBase must satisfy "
    "AnyPluginContentComponentBase.");


// TODO: panels

BLOOPER_NAMESPACE_END

#endif // BLOOPER_ABSTRACT_HPP
