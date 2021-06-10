#ifndef BLOOPER_JUCE_TRAITS_HPP
#define BLOOPER_JUCE_TRAITS_HPP

#include <blooper/internal/macros/macros.hpp>

#include "meta.hpp"
#include "traits.hpp"

BLOOPER_NAMESPACE_BEGIN

// Basic

using JuceFlag [[maybe_unused]] = juce::Atomic<bool>;

using JuceString [[maybe_unused]] = juce::String;
using JuceStringArray [[maybe_unused]] = juce::StringArray;

template<typename TObject>
using JuceRef [[maybe_unused]] = juce::ReferenceCountedObjectPtr<TObject>;

template<typename TObject>
// can't put const chere because of how juce does reference counting...
using JuceConstRef [[maybe_unused]] = juce::ReferenceCountedObjectPtr<TObject>;

using JuceValue [[maybe_unused]] = juce::Value;
using JuceVar [[maybe_unused]] = juce::var;
using JuceValueTree [[maybe_unused]] = juce::ValueTree;
using JuceXml [[maybe_unused]] = juce::XmlElement;
using JuceXmlDoc [[maybe_unused]] = juce::XmlDocument;
using JuceXmlFile [[maybe_unused]] = juce::PropertiesFile;
using JuceFile [[maybe_unused]] = juce::File;

template<typename TObject>
using JuceCached [[maybe_unused]] = juce::CachedValue<TObject>;

using JuceBounds [[maybe_unused]] = juce::Rectangle<int>;

using JuceTimer [[maybe_unused]] = juce::Timer;

// Assets

using JuceImage [[maybe_unused]] = juce::Image;
using JuceImageFileFormat [[maybe_unused]] = juce::ImageFileFormat;


// Graphics

using JuceGraphics [[maybe_unused]] = juce::Graphics;
using JuceDrawable [[maybe_unused]] = juce::Drawable;


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
using JuceProjectConstRef [[maybe_unused]] = JuceConstRef<te::Project>;

using JuceProjectItem [[maybe_unused]] = te::ProjectItem;
using JuceProjectItemId [[maybe_unused]] = te::ProjectItemID;
using JuceProjectItemRef [[maybe_unused]] = JuceRef<JuceProjectItem>;
using JuceProjectItemConstRef [[maybe_unused]] = JuceConstRef<JuceProjectItem>;

using JuceLogger [[maybe_unused]] = juce::Logger;

using JuceUndoManager [[maybe_unused]] = juce::UndoManager;
using JuceUndoableAction [[maybe_unused]] = juce::UndoableAction;

using JuceSelectionManager [[maybe_unused]] = te::SelectionManager;


[[maybe_unused]] inline constexpr auto isJuceLookAndFeel =
    meta::reverse_partial(
        meta::traits::is_convertible,
        meta::type_c<juce::LookAndFeel&&>);


using JuceLookAndFeel [[maybe_unused]] = juce::LookAndFeel;
using JuceColourId [[maybe_unused]] = int;
using JuceColour [[maybe_unused]] = juce::Colour;

BLOOPER_STATIC_ASSERT(
    isJuceLookAndFeel(meta::type_c<juce::LookAndFeel_V4>),
    "LookAndFeel_V4 must satisfy JuceLookAndFeel.");


// Commands

using JuceCommandManager [[maybe_unused]] = juce::ApplicationCommandManager;

using JuceCommandId [[maybe_unused]] =
    juce::CommandID;

using JuceCommand [[maybe_unused]] =
    juce::ApplicationCommandTarget::InvocationInfo;

using JuceCommandInfo [[maybe_unused]] =
    juce::ApplicationCommandInfo;


[[maybe_unused]] inline constexpr auto isJuceCommandTarget =
    meta::reverse_partial(
        meta::traits::is_convertible,
        meta::type_c<juce::ApplicationCommandTarget&&>);


using JuceCommandTarget [[maybe_unused]] = juce::ApplicationCommandTarget;

BLOOPER_STATIC_ASSERT(
    isJuceCommandTarget(meta::type_c<JuceCommandTarget>),
    "JuceCommandTarget must satisfy JuceCommandTarget.");


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


using JuceToolbarItemId [[maybe_unused]] = int;


// Window

[[maybe_unused]] inline constexpr auto isJuceWindow =
    meta::reverse_partial(
        meta::traits::is_convertible,
        meta::type_c<juce::DocumentWindow&&>);


using JuceWindow [[maybe_unused]] = juce::DocumentWindow;

BLOOPER_STATIC_ASSERT(
    isJuceWindow(meta::type_c<JuceWindow>),
    "JuceWindow must satisfy JuceWindow.");


// Edit

using JuceEdit [[maybe_unused]] = te::Edit;
using JuceEditRef [[maybe_unused]] = std::shared_ptr<JuceEdit>;
// for now the same as other juce refs because they work the same way
using JuceEditConstRef [[maybe_unused]] = std::shared_ptr<JuceEdit>;

using JuceTransport [[maybe_unused]] = te::TransportControl;
using JuceTransportRef [[maybe_unused]] = te::TransportControl*;
// for now the same as other juce refs because they work the same way
using JuceTransportConstRef [[maybe_unused]] = te::TransportControl*;


[[maybe_unused]] inline constexpr auto isJuceTrack =
    meta::reverse_partial(
        meta::traits::is_convertible,
        meta::type_c<te::Track&&>);


using JuceTrack [[maybe_unused]] = te::Track;
using JuceTrackRef [[maybe_unused]] = JuceRef<JuceTrack>;
using JuceTrackConstRef [[maybe_unused]] = JuceConstRef<JuceTrack>;

using JuceClipTrack [[maybe_unused]] = te::ClipTrack;
using JuceClipTrackRef [[maybe_unused]] = JuceRef<JuceClipTrack>;
using JuceClipTrackConstRef [[maybe_unused]] = JuceConstRef<JuceClipTrack>;

using JuceAudioTrack [[maybe_unused]] = te::AudioTrack;
using JuceAudioTrackRef [[maybe_unused]] = JuceRef<JuceAudioTrack>;
using JuceAudioTrackConstRef [[maybe_unused]] = JuceConstRef<JuceAudioTrack>;

BLOOPER_STATIC_ASSERT(
    isJuceTrack(meta::type_c<JuceTrack>),
    "JuceTrack must satisfy JuceTrack.");


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
