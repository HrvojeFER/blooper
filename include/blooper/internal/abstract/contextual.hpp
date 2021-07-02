#ifndef BLOOPER_CONTEXTUAL_HPP
#define BLOOPER_CONTEXTUAL_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>

#include <blooper/internal/abstract/meta.hpp>
#include <blooper/internal/abstract/traits.hpp>

#include <blooper/internal/abstract/juceTraits.hpp>

#include <blooper/internal/abstract/stateful.hpp>

BLOOPER_NAMESPACE_BEGIN

[[maybe_unused]] inline constexpr auto isAnyContext =
    meta::always(meta::true_c);

[[maybe_unused]] inline constexpr auto isCoreContext =
    meta::satisfies_all(
        isAnyStateful,
        isAnyContext,
        isJuceCommandTarget,
        meta::attribute(
            [](auto&& toCheck)
                -> decltype(meta::and_(
                    meta::traits::is_same(
                        meta::typeid_(toCheck.getRootDir()),
                        meta::type_c<JuceFile>),
                    meta::traits::is_same(
                        meta::typeid_(toCheck.getProjectsDir()),
                        meta::type_c<JuceFile>),

                    meta::traits::is_same(
                        meta::typeid_(toCheck.getEngineSettings()),
                        meta::type_c<JuceState>),

                    meta::traits::is_same(
                        meta::typeid_(toCheck.getSettings()),
                        meta::type_c<JuceState>),

                    meta::traits::is_same(
                        meta::typeid_(toCheck.getLogger()),
                        meta::type_c<JuceLogger>),
                    meta::traits::is_same(
                        meta::typeid_(toCheck.getAssetManager()),
                        meta::type_c<class AssetManager>),
                    meta::traits::is_same(
                        meta::typeid_(toCheck.getUndoManager()),
                        meta::type_c<JuceUndoManager>),
                    meta::traits::is_same(
                        meta::typeid_(toCheck.getUndoManagerPtr()),
                        meta::type_c<JuceUndoManager*>),
                    meta::traits::is_same(
                        meta::typeid_(toCheck.getCommandManager()),
                        meta::type_c<JuceCommandManager>),

                    meta::traits::is_same(
                        meta::typeid_(toCheck.getSelectionManager()),
                        meta::type_c<JuceSelectionManager>),
                    meta::traits::is_same(
                        meta::typeid_(toCheck.getSelectionManagerPtr()),
                        meta::type_c<JuceSelectionManager*>),
                    meta::traits::is_same(
                        meta::typeid_(toCheck.getFocusedSelectionManager()),
                        meta::type_c<JuceSelectionManagerRef>),
                    meta::traits::is_same(
                        meta::typeid_(toCheck.setFocusedSelectionManager(
                            std::declval<JuceSelectionManagerRef>())),
                        meta::type_c<JuceSelectionManagerRef>),

                    meta::traits::is_same(
                        meta::typeid_(toCheck.getLookAndFeel()),
                        meta::type_c<JuceLookAndFeel>),

                    meta::traits::is_same(
                        meta::typeid_(toCheck.getEngine()),
                        meta::type_c<JuceEngine>))) {}) ^
            meta::after ^
            meta::check(
                [](auto&& toCheck)
                    -> decltype(toCheck.getRootDir(),
                                toCheck.getProjectsDir(),

                                toCheck.getEngineSettings(),
                                toCheck.saveEngineSettings(),

                                toCheck.getSettings(),

                                toCheck.getLogger(),
                                toCheck.log(std::declval<juce::String>()),

                                toCheck.getAssetManager(),
                                toCheck.getUndoManager(),
                                toCheck.getUndoManagerPtr(),

                                toCheck.getCommandManager(),
                                toCheck.registerCommandTarget(
                                    std::declval<JuceCommandTarget*>()),
                                toCheck.unregisterCommandTarget(
                                    std::declval<JuceCommandTarget*>()),

                                toCheck.getSelectionManager(),
                                toCheck.getSelectionManagerPtr(),
                                toCheck.getFocusedSelectionManager(),
                                toCheck.setFocusedSelectionManager(
                                    std::declval<JuceSelectionManagerRef>()),

                                toCheck.getLookAndFeel(),

                                toCheck.getEngine()) {}));

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
                        meta::typeid_(toCheck.getProjectRef()),
                        meta::type_c<JuceProjectRef>),

                    meta::traits::is_same(
                        meta::typeid_(toCheck.getProjectSettings()),
                        meta::type_c<JuceState>),
                    meta::traits::is_same(
                        meta::typeid_(toCheck.getProjectState()),
                        meta::type_c<JuceState>),

                    meta::traits::is_same(
                        meta::typeid_(toCheck.getEditManager()),
                        meta::type_c<class EditManager>),
                    meta::traits::is_same(
                        meta::typeid_(toCheck.getFocusedEdit()),
                        meta::type_c<JuceEditRef>),

                    meta::traits::is_same(
                        meta::typeid_(toCheck.getEditManager()),
                        meta::type_c<class EditManager>),


                    meta::traits::is_same(
                        meta::typeid_(toCheck.getSynchronizer()),
                        meta::type_c<class Synchronizer>))) {}) ^
            meta::after ^
            meta::check(
                [](auto&& toCheck)
                    -> decltype(toCheck.getProject(),
                                toCheck.getProjectRef(),

                                toCheck.getProjectSettings(),
                                toCheck.getProjectState(),

                                toCheck.getEditManager(),
                                toCheck.getFocusedEdit(),
                                toCheck.setFocusedEdit(
                                    std::declval<JuceEditRef>()),

                                toCheck.getSynchronizer()) {}));

template<typename TStatefulTraits>
class [[maybe_unused]] AnyAbstractCoreContext :
    public virtual TStatefulTraits::abstractType,
    public virtual juce::ApplicationCommandTarget
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
  [[maybe_unused]] inline AnyAbstractCoreContext() = default;

  [[maybe_unused]] inline ~AnyAbstractCoreContext() override = default;


  [[maybe_unused, nodiscard]] virtual inline const JuceFile&
  getRootDir() const noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline JuceFile&
  getRootDir() noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline const JuceFile&
  getProjectsDir() const noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline JuceFile&
  getProjectsDir() noexcept = 0;


  [[maybe_unused, nodiscard]] virtual inline const JuceState&
  getEngineSettings() const noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline JuceState&
  getEngineSettings() noexcept = 0;

  [[maybe_unused]] virtual void
  saveEngineSettings() = 0;

  [[maybe_unused, nodiscard]] virtual inline const JuceState&
  getSettings() const noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline JuceState&
  getSettings() noexcept = 0;


  [[maybe_unused, nodiscard]] virtual inline const JuceLogger&
  getLogger() const noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline JuceLogger&
  getLogger() noexcept = 0;

  [[maybe_unused]] virtual void
  log(const JuceString& message) = 0;


  [[maybe_unused, nodiscard]] virtual inline const class AssetManager&
  getAssetManager() const noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline class AssetManager&
  getAssetManager() noexcept = 0;


  [[maybe_unused, nodiscard]] virtual inline const JuceUndoManager&
  getUndoManager() const noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline JuceUndoManager&
  getUndoManager() noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline const JuceUndoManager*
  getUndoManagerPtr() const noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline JuceUndoManager*
  getUndoManagerPtr() noexcept = 0;


  [[maybe_unused, nodiscard]] virtual inline const JuceCommandManager&
  getCommandManager() const noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline JuceCommandManager&
  getCommandManager() noexcept = 0;

  [[maybe_unused]] virtual void
  registerCommandTarget(JuceCommandTarget* target) = 0;

  [[maybe_unused]] virtual void
  unregisterCommandTarget(JuceCommandTarget* target) = 0;


  [[maybe_unused, nodiscard]] virtual inline const JuceSelectionManager&
  getSelectionManager() const noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline JuceSelectionManager&
  getSelectionManager() noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline const JuceSelectionManager*
  getSelectionManagerPtr() const noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline JuceSelectionManager*
  getSelectionManagerPtr() noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline JuceSelectionManagerRef
  getFocusedSelectionManager() = 0;

  [[maybe_unused]] virtual inline JuceSelectionManagerRef
      setFocusedSelectionManager(JuceSelectionManagerRef) = 0;


  [[maybe_unused, nodiscard]] virtual inline const JuceLookAndFeel&
  getLookAndFeel() const noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline JuceLookAndFeel&
  getLookAndFeel() noexcept = 0;


  [[maybe_unused, nodiscard]] virtual inline const JuceEngine&
  getEngine() const noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline JuceEngine&
  getEngine() noexcept = 0;


  [[maybe_unused]] virtual inline void
      openProject(JuceProjectRef) = 0;
};

template<typename TStatefulTraits>
class [[maybe_unused]] AnyAbstractContext :
    public virtual AnyAbstractCoreContext<TStatefulTraits>
{
 public:
  [[maybe_unused]] inline AnyAbstractContext() = default;

  [[maybe_unused]] inline ~AnyAbstractContext() override = default;


  [[maybe_unused, nodiscard]] virtual inline const JuceProject&
  getProject() const noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline JuceProject&
  getProject() noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline JuceProjectConstRef
  getProjectRef() const noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline JuceProjectRef
  getProjectRef() noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline const JuceState&
  getProjectSettings() const noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline JuceState&
  getProjectSettings() noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline const JuceState&
  getProjectState() const noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline JuceState&
  getProjectState() noexcept = 0;


  [[maybe_unused, nodiscard]] virtual inline const class EditManager&
  getEditManager() const noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline class EditManager&
  getEditManager() noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline JuceEditRef
  getFocusedEdit() = 0;

  [[maybe_unused]] virtual inline JuceEditRef
      setFocusedEdit(JuceEditRef) = 0;


  [[maybe_unused, nodiscard]] virtual inline const class Synchronizer&
  getSynchronizer() const noexcept = 0;

  [[maybe_unused, nodiscard]] virtual inline class Synchronizer&
  getSynchronizer() noexcept = 0;
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
    meta::after ^ isAnyContextual;

[[maybe_unused]] inline constexpr auto isContextual =
    meta::attribute(
        [](auto&& toCheck)
            -> decltype(isContext(
                meta::typeid_(toCheck.getContext()))) {}) ^
    meta::after ^ isAnyContextual;

[[maybe_unused]] inline constexpr auto isCoreContextualBase =
    meta::satisfies_all(
        meta::reverse_partial(
            meta::traits::is_constructible,
            meta::type_c<AnyAbstractCoreContext<StatefulTraits>&>),
        isCoreContextual);

[[maybe_unused]] inline constexpr auto isContextualBase =
    meta::satisfies_all(
        meta::reverse_partial(
            meta::traits::is_constructible,
            meta::type_c<AnyAbstractContext<StatefulTraits>&>),
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
    meta::after ^
    meta::check(
        [](auto&& toCheck)
            -> decltype(meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         contextType>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         abstractType>,
                        meta::type_c<typename std::decay_t<decltype(toCheck)>::
                                         baseType>) {});

template<typename TContextualTraits>
class AnyAbstractContextual;

template<typename TContextualTraits>
class AnyContextualBase;

template<typename TContext, typename = void>
struct AnyContextualTraits
{
  static_assert(
      isAnyContext(meta::type_c<TContext>),
      "AnyContextualTraits requires AnyContext.");


  using contextType = TContext;

  using abstractType = AnyAbstractContextual<AnyContextualTraits>;
  using baseType = AnyContextualBase<AnyContextualTraits>;
};


template<typename TContextualTraits>
class [[maybe_unused]] AnyAbstractContextual
{
  using contextType [[maybe_unused]] =
      typename TContextualTraits::contextType;

  static_assert(
      isAnyContext(meta::type_c<contextType>),
      "AbstractAnyContextual requires AnyContext.");


 public:
  [[maybe_unused]] inline AnyAbstractContextual() = default;

  [[maybe_unused]] virtual inline ~AnyAbstractContextual() = default;


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
        AnyAbstractCoreContext<
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
        AnyAbstractContext<
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


// Explicit instantiation

// context

extern template class AnyAbstractCoreContext<StatefulTraits>;
extern template class AnyAbstractContext<StatefulTraits>;

// contextual

extern template class AnyAbstractContextual<CoreContextualTraits>;
extern template class AnyAbstractContextual<ContextualTraits>;

extern template class AnyContextualBase<CoreContextualTraits>;
extern template class AnyContextualBase<ContextualTraits>;

BLOOPER_NAMESPACE_END

#endif // BLOOPER_CONTEXTUAL_HPP
