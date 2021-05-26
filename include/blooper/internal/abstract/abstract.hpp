#ifndef BLOOPER_ABSTRACT_HPP
#define BLOOPER_ABSTRACT_HPP


#include <blooper/internal/macros/macros.hpp>

#include <blooper/internal/abstract/meta.hpp>
#include <blooper/internal/abstract/traits.hpp>

#include <blooper/internal/abstract/juceTraits.hpp>


// TODO: concepts (Core)?(Contextual|Component|Window),
//  state/listener/identifier abstractions,
//  base construction assumptions,
//  UndoManager management


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

template<typename TState>
[[maybe_unused]] inline constexpr auto isAnyStatefulBase =
    meta::satisfies_all(
        meta::reverse_partial(
            meta::traits::is_constructible,
            meta::type_c<TState>),
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
                isAnyStatefulBase<
                    typename std::decay_t<decltype(toCheck)>::
                        stateType>(
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

template<typename TState>
class AbstractAnyStateful;

template<typename TState>
class AnyStatefulBase;

class StatefulBase;

template<typename TState>
struct AnyStatefulTraits
{
  static_assert(
      isAnyState(meta::type_c<TState>),
      "AnyStatefulTraits requires AnyState.");


  using stateType = TState;

  using abstractType = AbstractAnyStateful<stateType>;
  using baseType = AnyStatefulBase<stateType>;
};

template<>
struct AnyStatefulTraits<State>
{
  using stateType = State;

  using abstractType = AbstractAnyStateful<stateType>;
  using baseType = StatefulBase;
};


template<typename TState>
class [[maybe_unused]] AbstractAnyStateful
{
  static_assert(
      isAnyState(meta::type_c<TState>),
      "AbstractAnyStateful requires AnyState.");

 public:
  [[maybe_unused]] inline AbstractAnyStateful() = default;

  [[maybe_unused]] virtual inline ~AbstractAnyStateful() = default;


  [[maybe_unused, nodiscard]] virtual inline const TState&
  getState() const noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline TState&
  getState() noexcept = 0;
};

template<typename TState>
class [[maybe_unused]] AnyStatefulBase :
    public virtual AnyStatefulTraits<TState>::abstractType
{
  static_assert(
      isAnyState(meta::type_c<TState>),
      "AbstractAnyStateful requires AnyState.");


  using abstractType [[maybe_unused]] =
      typename AnyStatefulTraits<TState>::abstractType;


 public:
  [[maybe_unused]] inline explicit AnyStatefulBase(TState state)
      : state(std::move(state))
  {
  }


  [[maybe_unused, nodiscard]] inline const TState&
  getState() const noexcept final
  {
    return state;
  }

  [[maybe_unused, nodiscard]] inline TState&
  getState() noexcept final
  {
    return state;
  }


 protected:
  [[maybe_unused]] TState state;
};

class StatefulBase :
    public AnyStatefulBase<State>,
    protected StateListener
{
 public:
  explicit StatefulBase(State state)
      : AnyStatefulBase<State>(std::move(state))
  {
    getState().addListener(this);
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

template<typename TState>
class [[maybe_unused]] AbstractAnyCoreContext :
    public virtual AnyStatefulTraits<TState>::abstractType
{
  static_assert(
      isAnyState(meta::type_c<TState>),
      "AbstractAnyCoreContext requires AnyState.");


  using abstractStateful [[maybe_unused]] =
      typename AnyStatefulTraits<TState>::abstractType;


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

template<typename TState>
class [[maybe_unused]] AbstractAnyContext :
    public virtual AbstractAnyCoreContext<TState>
{
  static_assert(
      isAnyState(meta::type_c<TState>),
      "AbstractAnyContext requires AnyState.");


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

template<typename TContext>
[[maybe_unused]] inline constexpr auto isAnyContextualBase =
    meta::satisfies_all(
        meta::reverse_partial(
            meta::traits::is_constructible,
            meta::type_c<TContext&>),
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
            meta::type_c<AbstractAnyCoreContext<State>&>),
        isCoreContextual);

[[maybe_unused]] inline constexpr auto isContextualBase =
    meta::satisfies_all(
        meta::reverse_partial(
            meta::traits::is_constructible,
            meta::type_c<AbstractAnyContext<State>&>),
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
                isAnyContextualBase<typename std::decay_t<decltype(toCheck)>::
                                        contextType>(
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

template<typename TContext>
struct AnyContextualTraits;

template<typename TContext>
class AbstractAnyContextual;

template<typename TContext>
class AnyContextualBase;

template<typename TContext>
struct AnyContextualTraits
{
  static_assert(
      isAnyContext(meta::type_c<TContext>),
      "AnyContextualTraits requires AnyContext.");


  using contextType = TContext;

  using abstractType = AbstractAnyContextual<contextType>;
  using baseType = AnyContextualBase<contextType>;
};


template<typename TContext>
class [[maybe_unused]] AbstractAnyContextual
{
  static_assert(
      isAnyContext(meta::type_c<TContext>),
      "AbstractAnyContextual requires AnyContext.");


 public:
  [[maybe_unused]] inline AbstractAnyContextual() = default;

  [[maybe_unused]] virtual inline ~AbstractAnyContextual() = default;


  [[maybe_unused, nodiscard]] virtual inline const TContext&
  getContext() const noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline TContext&
  getContext() noexcept = 0;
};

template<typename TContext>
class [[maybe_unused]] AnyContextualBase :
    public virtual AnyContextualTraits<TContext>::abstractType
{
  static_assert(
      isAnyContext(meta::type_c<TContext>),
      "AnyContextualBase requires AnyContext.");


  using abstractType [[maybe_unused]] =
      typename AnyContextualTraits<TContext>::abstractType;


 public:
  [[maybe_unused]] inline explicit AnyContextualBase(TContext& context)
      : context(context)
  {
  }


  [[maybe_unused, nodiscard]] inline const TContext&
  getContext() const noexcept final
  {
    return context;
  }

  [[maybe_unused, nodiscard]] inline TContext&
  getContext() noexcept final
  {
    return context;
  }


 protected:
  [[maybe_unused]] TContext& context;
};


using CoreContextualTraits =
    AnyContextualTraits<AbstractAnyCoreContext<State>>;

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
    AnyContextualTraits<AbstractAnyContext<State>>;

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
        isJuceComponent);

template<typename TContext, typename TState>
[[maybe_unused]] inline constexpr auto isAnyComponentBase =
    meta::satisfies_all(
        meta::reverse_partial(
            meta::traits::is_constructible,
            meta::type_c<TContext&>,
            meta::type_c<TState>),
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
                isAnyContext(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     contextType>),
                isAnyState(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     stateType>),
                isJuceComponent(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     juceBaseType>),

                isAnyComponent(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     abstractType>),
                isAnyComponentBase<
                    typename std::decay_t<decltype(toCheck)>::
                        contextType,
                    typename std::decay_t<decltype(toCheck)>::
                        stateType>(
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
                                         contextType>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         stateType>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         juceBaseType>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         abstractType>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         baseType>) {});

template<typename TContext, typename TState, typename TJuceBase = JuceComponent>
class AnyAbstractComponent;

template<typename TContext, typename TState, typename TJuceBase = JuceComponent>
class AnyComponentBase;

template<typename TContext, typename TState, typename TJuceBase = JuceComponent>
struct AnyComponentTraits
{
  static_assert(
      isAnyContext(meta::type_c<TContext>),
      "AnyComponentTraits requires AnyContext.");

  static_assert(
      isAnyState(meta::type_c<TState>),
      "AnyComponentTraits requires AnyState.");

  static_assert(
      isJuceComponent(meta::type_c<TJuceBase>),
      "AnyComponentTraits requires a JuceComponent.");


  using contextType = TContext;
  using stateType = TState;

  using juceBaseType = TJuceBase;

  using abstractType = AnyAbstractComponent<
      contextType,
      stateType,
      juceBaseType>;

  using baseType = AnyComponentBase<
      contextType,
      stateType,
      juceBaseType>;
};


template<typename TContext, typename TState, typename TJuceBase>
class [[maybe_unused]] AnyAbstractComponent :
    public virtual AnyContextualTraits<TContext>::abstractType,
    public virtual AnyStatefulTraits<TState>::abstractType,
    public TJuceBase
{
  static_assert(
      isJuceComponent(meta::type_c<TJuceBase>),
      "AnyAbstractComponent requires a JuceComponent.");


  using contextualBase [[maybe_unused]] =
      typename AnyContextualTraits<TContext>::abstractType;

  using statefulBase [[maybe_unused]] =
      typename AnyStatefulTraits<TState>::abstractType;


 public:
  [[maybe_unused]] inline AnyAbstractComponent() = default;

  [[maybe_unused]] inline ~AnyAbstractComponent() override = default;
};

template<typename TContext, typename TState, typename TJuceBase>
class [[maybe_unused]] AnyComponentBase :
    public virtual AnyComponentTraits<TContext, TState, TJuceBase>::abstractType,
    public AnyContextualTraits<TContext>::baseType,
    public AnyStatefulTraits<TState>::baseType
{
  static_assert(
      isJuceComponent(meta::type_c<TJuceBase>),
      "AnyComponentBase requires a JuceComponent.");


  using abstractType [[maybe_unused]] =
      typename AnyComponentTraits<TContext, TState, TJuceBase>::abstractType;

  using contextualBase [[maybe_unused]] =
      typename AnyContextualTraits<TContext>::baseType;

  using statefulBase [[maybe_unused]] =
      typename AnyStatefulTraits<TState>::baseType;


 public:
  [[maybe_unused]] inline explicit AnyComponentBase(
      TContext& context,
      TState    state)
      : contextualBase(context),
        statefulBase(std::move(state))
  {
    // TODO: defaults here
  }
};


using CoreComponentTraits = AnyComponentTraits<
    AbstractCoreContext,
    State,
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

using ComponentTraits = AnyComponentTraits<
    AbstractContext,
    State,
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
        isJuceWindow);

template<typename TContext, typename TState>
[[maybe_unused]] inline constexpr auto isAnyWindowBase =
    meta::satisfies_all(
        meta::reverse_partial(
            meta::traits::is_constructible,
            meta::type_c<JuceString>,
            meta::type_c<TContext&>,
            meta::type_c<TState>),
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
                isAnyContext(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     contextType>),
                isAnyState(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     stateType>),
                isJuceWindow(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     juceBaseType>),

                isAnyWindow(
                    meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                     abstractType>),
                isAnyWindowBase<
                    typename std::decay_t<decltype(toCheck)>::
                        contextType,
                    typename std::decay_t<decltype(toCheck)>::
                        stateType>(
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
                                         contextType>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         stateType>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         juceBaseType>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         abstractType>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         baseType>) {});

template<typename TContext, typename TState, typename TJuceBase = JuceWindow>
class AnyAbstractWindow;

template<typename TContext, typename TState, typename TJuceBase = JuceWindow>
class AnyWindowBase;

template<typename TContext, typename TState, typename TJuceBase = JuceWindow>
struct [[maybe_unused]] AnyWindowTraits
{
  static_assert(
      isAnyContext(meta::type_c<TContext>),
      "AnyWindowTraits requires AnyContext.");

  static_assert(
      isAnyState(meta::type_c<TState>),
      "AnyWindowTraits requires AnyState.");

  static_assert(
      isJuceWindow(meta::type_c<TJuceBase>),
      "AnyWindowTraits requires a JuceWindow.");


  using contextType [[maybe_unused]] = TContext;
  using stateType [[maybe_unused]] = TState;

  using juceBaseType [[maybe_unused]] = TJuceBase;

  using abstractType [[maybe_unused]] = AnyAbstractWindow<
      contextType,
      stateType,
      juceBaseType>;

  using baseType [[maybe_unused]] = AnyWindowBase<
      contextType,
      stateType,
      juceBaseType>;
};


template<typename TContext, typename TState, typename TJuceBase>
class [[maybe_unused]] AnyAbstractWindow :
    public virtual AnyContextualTraits<TContext>::abstractType,
    public virtual AnyStatefulTraits<TState>::abstractType,
    public TJuceBase
{
  static_assert(
      isJuceWindow(meta::type_c<TJuceBase>),
      "AnyAbstractWindow requires a JuceWindow.");


  using abstractContextual [[maybe_unused]] =
      typename AnyContextualTraits<TContext>::abstractType;

  using abstractStateful [[maybe_unused]] =
      typename AnyStatefulTraits<TState>::abstractType;


 public:
  [[maybe_unused]] inline AnyAbstractWindow()
      : TJuceBase({}, {}, {})
  {
  }

  [[maybe_unused]] inline ~AnyAbstractWindow() override = default;
};

template<typename TContext, typename TState, typename TJuceBase>
class [[maybe_unused]] AnyWindowBase :
    public virtual AnyWindowTraits<TContext, TState, TJuceBase>::abstractType,
    public AnyContextualTraits<TContext>::baseType,
    public AnyStatefulTraits<TState>::baseType
{
  static_assert(
      isJuceWindow(meta::type_c<TJuceBase>),
      "AnyWindowBase requires a JuceWindow.");


  using abstractType [[maybe_unused]] =
      typename AnyWindowTraits<TContext, TState, TJuceBase>::abstractType;

  using contextualBase [[maybe_unused]] =
      typename AnyContextualTraits<TContext>::baseType;

  using statefulBase [[maybe_unused]] =
      typename AnyStatefulTraits<TState>::baseType;


 public:
  [[maybe_unused]] inline explicit AnyWindowBase(
      JuceString name,
      TContext&  context,
      TState     state)
      : contextualBase(context),
        statefulBase(std::move(state))
  {
    this->setName(std::move(name));

    // TODO: defaults here
  }
};


using CoreWindowTraits = AnyWindowTraits<
    AbstractCoreContext,
    State,
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

using WindowTraits = AnyWindowTraits<
    AbstractContext,
    State,
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


// id

#define BLOOPER_STATE_ID(_name)               \
  /* NOLINTNEXTLINE(cert-err58-cpp) */        \
  inline static const StateIdentifier stateId \
  {                                           \
    #_name                                    \
  }

#define BLOOPER_ID(_name)                   \
  /* NOLINTNEXTLINE(cert-err58-cpp) */      \
  inline static const StateIdentifier _name \
  {                                         \
    #_name                                  \
  }

namespace detail::id
{
BLOOPER_STATE_ID(unusedMacroWorkaround);
BLOOPER_ID(unusedMacroWorkaround);
} // namespace detail::id

BLOOPER_NAMESPACE_END

#endif // BLOOPER_ABSTRACT_HPP
