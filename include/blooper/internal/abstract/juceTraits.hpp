#ifndef BLOOPER_JUCE_TRAITS_HPP
#define BLOOPER_JUCE_TRAITS_HPP

#include <blooper/internal/macros/macros.hpp>

#include "meta.hpp"
#include "traits.hpp"

BLOOPER_NAMESPACE_BEGIN

// Basic

using JuceString [[maybe_unused]] = juce::String;
using JuceXml [[maybe_unused]] = juce::XmlElement;
using JuceXmlFile [[maybe_unused]] = juce::PropertiesFile;
using JuceBounds [[maybe_unused]] = juce::Rectangle<int>;

template<typename TObject>
using JuceRef [[maybe_unused]] = juce::ReferenceCountedObjectPtr<TObject>;

template<typename TObject>
using JuceConstRef [[maybe_unused]] = juce::ReferenceCountedObjectPtr<TObject>;


// State

[[maybe_unused]] inline constexpr auto isJuceState =
    meta::reverse_partial(
        meta::traits::is_convertible,
        meta::type_c<juce::ValueTree&&>);


using JuceState [[maybe_unused]] = juce::ValueTree;

BLOOPER_STATIC_ASSERT(
    isJuceState(meta::type_c<JuceState>),
    "JuceState must satisfy JuceState.");


using JuceStateListener [[maybe_unused]] = te::ValueTreeAllEventListener;
using JuceStateIdentifier [[maybe_unused]] = juce::Identifier;

template<typename TObject>
using JuceStateValue [[maybe_unused]] = juce::CachedValue<TObject>;


// Context

using JuceEngine [[maybe_unused]] = te::Engine;

using JuceProject [[maybe_unused]] = te::Project;
using JuceProjectRef [[maybe_unused]] = JuceRef<te::Project>;
// can't put const chere because of how juce does reference counting...
using JuceProjectConstRef [[maybe_unused]] = JuceConstRef<te::Project>;

using JuceEdit [[maybe_unused]] = te::Edit;
using JuceTransport [[maybe_unused]] = te::TransportControl;

using JuceUndoManager [[maybe_unused]] = juce::UndoManager;


[[maybe_unused]] inline constexpr auto isJuceLookAndFeel =
    meta::reverse_partial(
        meta::traits::is_convertible,
        meta::type_c<juce::LookAndFeel&&>);


using JuceLookAndFeel [[maybe_unused]] = juce::LookAndFeel;

BLOOPER_STATIC_ASSERT(
    isJuceLookAndFeel(meta::type_c<juce::LookAndFeel_V4>),
    "LookAndFeel_V4 must satisfy JuceLookAndFeel.");


// Component

[[maybe_unused]] inline constexpr auto isJuceComponent =
    meta::reverse_partial(
        meta::traits::is_convertible,
        meta::type_c<juce::Component&&>);


using JuceComponent [[maybe_unused]] = juce::Component;
using JuceConstrainer [[maybe_unused]] = juce::ComponentBoundsConstrainer;

BLOOPER_STATIC_ASSERT(
    isJuceComponent(meta::type_c<JuceComponent>),
    "JuceComponent must satisfy JuceComponent.");


// Window

[[maybe_unused]] inline constexpr auto isJuceWindow =
    meta::reverse_partial(
        meta::traits::is_convertible,
        meta::type_c<juce::DocumentWindow&&>);


using JuceWindow [[maybe_unused]] = juce::DocumentWindow;

BLOOPER_STATIC_ASSERT(
    isJuceWindow(meta::type_c<JuceWindow>),
    "JuceWindow must satisfy JuceWindow.");


// Plugin

[[maybe_unused]] inline constexpr auto isJucePlugin =
    meta::reverse_partial(
        meta::traits::is_convertible,
        meta::type_c<te::Plugin&&>);

[[maybe_unused]] inline constexpr auto isJucePluginRef =
    meta::compose(
        meta::reverse_partial(
            meta::traits::is_convertible,
            meta::type_c<te::Plugin&>),
        meta::traits::dereference);

[[maybe_unused]] inline constexpr auto isJucePluginConstRef =
    meta::compose(
        meta::reverse_partial(
            meta::traits::is_convertible,
            meta::type_c<const te::Plugin&>),
        meta::traits::dereference);


using JucePlugin [[maybe_unused]] = te::Plugin;
using JucePluginRef [[maybe_unused]] = JuceRef<JucePlugin>;
// can't put const chere because of how juce does reference counting...
using JucePluginConstRef [[maybe_unused]] = JuceConstRef<JucePlugin>;

using JuceExternalPlugin [[maybe_unused]] = te::ExternalPlugin;
using JuceExternalPluginRef [[maybe_unused]] = JuceRef<JuceExternalPlugin>;
// can't put const chere because of how juce does reference counting...
using JuceExternalPluginConstRef [[maybe_unused]] = JuceConstRef<JuceExternalPlugin>;

using JucePluginWindowState [[maybe_unused]] = te::PluginWindowState;

BLOOPER_STATIC_ASSERT(
    isJucePlugin(meta::type_c<JucePlugin>),
    "JucePlugin must satisfy JucePlugin.");

BLOOPER_STATIC_ASSERT(
    isJucePlugin(meta::type_c<JuceExternalPlugin>),
    "JuceExternalPlugin must satisfy JucePlugin.");

BLOOPER_STATIC_ASSERT(
    isJucePluginRef(meta::type_c<JucePluginRef>),
    "JucePluginRef must satisfy JucePluginRef.");

BLOOPER_STATIC_ASSERT(
    isJucePluginConstRef(meta::type_c<JucePluginConstRef>),
    "JucePluginConstRef must satisfy JucePluginConstRef.");

BLOOPER_STATIC_ASSERT(
    isJucePluginRef(meta::type_c<JuceExternalPluginRef>),
    "JuceExternalPluginRef must satisfy JucePluginRef.");

BLOOPER_STATIC_ASSERT(
    isJucePluginConstRef(meta::type_c<JuceExternalPluginConstRef>),
    "JuceExternalPluginConstRef must satisfy JucePluginConstRef.");


using JucePluginContent [[maybe_unused]] =
    juce::AudioProcessorEditor;

using JuceGenericPluginContent [[maybe_unused]] =
    juce::GenericAudioProcessorEditor;

BLOOPER_NAMESPACE_END

#endif // BLOOPER_JUCE_TRAITS_HPP
