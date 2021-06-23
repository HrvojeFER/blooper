#ifndef BLOOPER_PLUGINS_HPP
#define BLOOPER_PLUGINS_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>

#include <blooper/internal/abstract/meta.hpp>
#include <blooper/internal/abstract/traits.hpp>
#include <blooper/internal/abstract/juceTraits.hpp>

#include <blooper/internal/abstract/stateful.hpp>
#include <blooper/internal/abstract/contextual.hpp>

#include <blooper/internal/abstract/components.hpp>

BLOOPER_NAMESPACE_BEGIN

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


// Explicit instantiation

// content

extern template class AnyAbstractPluginContentComponent<
    PluginContentTraits>;

extern template class AnyPluginContentComponentBase<
    PluginContentTraits>;

extern template class AnyPluginContentComponentBase<
    PluginContentTraits,
    ExternalPluginTraits>;

// effects

extern template class AnyPluginContentComponentBase<
    PluginContentTraits,
    AnyPluginTraits<te::VolumeAndPanPlugin>>;

extern template class AnyPluginContentComponentBase<
    PluginContentTraits,
    AnyPluginTraits<te::EqualiserPlugin>>;

// TODO: fix
//extern template class AnyPluginContentComponentBase<
//    PluginContentTraits,
//    AnyPluginTraits<te::ReverbPlugin>>;

extern template class AnyPluginContentComponentBase<
    PluginContentTraits,
    AnyPluginTraits<te::DelayPlugin>>;

extern template class AnyPluginContentComponentBase<
    PluginContentTraits,
    AnyPluginTraits<te::ChorusPlugin>>;

extern template class AnyPluginContentComponentBase<
    PluginContentTraits,
    AnyPluginTraits<te::PhaserPlugin>>;

extern template class AnyPluginContentComponentBase<
    PluginContentTraits,
    AnyPluginTraits<te::CompressorPlugin>>;

extern template class AnyPluginContentComponentBase<
    PluginContentTraits,
    AnyPluginTraits<te::PitchShiftPlugin>>;

extern template class AnyPluginContentComponentBase<
    PluginContentTraits,
    AnyPluginTraits<te::LowPassPlugin>>;

extern template class AnyPluginContentComponentBase<
    PluginContentTraits,
    AnyPluginTraits<te::MidiModifierPlugin>>;

extern template class AnyPluginContentComponentBase<
    PluginContentTraits,
    AnyPluginTraits<te::MidiPatchBayPlugin>>;

extern template class AnyPluginContentComponentBase<
    PluginContentTraits,
    AnyPluginTraits<te::PatchBayPlugin>>;

extern template class AnyPluginContentComponentBase<
    PluginContentTraits,
    AnyPluginTraits<te::AuxSendPlugin>>;

extern template class AnyPluginContentComponentBase<
    PluginContentTraits,
    AnyPluginTraits<te::AuxReturnPlugin>>;

extern template class AnyPluginContentComponentBase<
    PluginContentTraits,
    AnyPluginTraits<te::TextPlugin>>;

extern template class AnyPluginContentComponentBase<
    PluginContentTraits,
    AnyPluginTraits<te::FreezePointPlugin>>;

extern template class AnyPluginContentComponentBase<
    PluginContentTraits,
    AnyPluginTraits<te::InsertPlugin>>;

// synths

// TODO: fix
//extern template class AnyPluginContentComponentBase<
//    PluginContentTraits,
//    AnyPluginTraits<te::FourOscPlugin>>;

extern template class AnyPluginContentComponentBase<
    PluginContentTraits,
    AnyPluginTraits<te::SamplerPlugin>>;

BLOOPER_NAMESPACE_END

#endif // BLOOPER_PLUGINS_HPP
