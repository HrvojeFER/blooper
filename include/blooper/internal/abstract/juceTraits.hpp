#ifndef BLOOPER_JUCE_TRAITS_HPP
#define BLOOPER_JUCE_TRAITS_HPP

#include <blooper/internal/macros/macros.hpp>

BLOOPER_NAMESPACE_BEGIN

// Basic

using JuceString [[maybe_unused]] = juce::String;
using JuceXml [[maybe_unused]] = juce::XmlElement;
using JuceXmlFile [[maybe_unused]] = juce::PropertiesFile;


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


// Context

[[maybe_unused]] inline constexpr auto isJuceLookAndFeel =
    meta::reverse_partial(
        meta::traits::is_convertible,
        meta::type_c<juce::LookAndFeel&&>);


using JuceEngine [[maybe_unused]] = te::Engine;
using JuceUndoManager [[maybe_unused]] = juce::UndoManager;
using JuceLookAndFeel [[maybe_unused]] = juce::LookAndFeel;

using JuceProject [[maybe_unused]] = te::Project;
using JuceEdit [[maybe_unused]] = te::Edit;
using JuceTransport [[maybe_unused]] = te::TransportControl;

BLOOPER_STATIC_ASSERT(
    isJuceLookAndFeel(meta::type_c<juce::LookAndFeel_V4>),
    "LookAndFeel_V4 must satisfy JuceLookAndFeel.");


// Component

[[maybe_unused]] inline constexpr auto isJuceComponent =
    meta::reverse_partial(
        meta::traits::is_convertible,
        meta::type_c<juce::Component&&>);


using JuceComponent [[maybe_unused]] = juce::Component;

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

BLOOPER_NAMESPACE_END

#endif // BLOOPER_JUCE_TRAITS_HPP
