#ifndef BLOOPER_JUCE_TRAITS_HPP
#define BLOOPER_JUCE_TRAITS_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>

#include <blooper/internal/abstract/prelude.hpp>
#include <blooper/internal/abstract/meta.hpp>
#include <blooper/internal/abstract/traits.hpp>

BLOOPER_NAMESPACE_BEGIN

// Basic

using JuceFlag [[maybe_unused]] = juce::Atomic<bool>;

using JuceString [[maybe_unused]] = juce::String;
using JuceStringRef [[maybe_unused]] = juce::StringRef;
using JuceStringArray [[maybe_unused]] = juce::StringArray;

template<typename TObject>
using JuceRef [[maybe_unused]] = juce::ReferenceCountedObjectPtr<TObject>;

template<typename TObject>
// can't put const here because of how juce does reference counting...
using JuceConstRef [[maybe_unused]] = juce::ReferenceCountedObjectPtr<TObject>;


// Files

using JuceFile [[maybe_unused]] = juce::File;

using JuceXml [[maybe_unused]] = juce::XmlElement;
using JuceXmlDoc [[maybe_unused]] = juce::XmlDocument;
using JuceXmlFile [[maybe_unused]] = juce::PropertiesFile;


// Threads

using JuceTimer [[maybe_unused]] = juce::Timer;


// Math

using JuceBounds [[maybe_unused]] = juce::Rectangle<int>;
using JuceTimeRange [[maybe_unused]] = te::EditTimeRange;

struct [[maybe_unused]] BoundsAndTime
{
  [[maybe_unused]] bool isValid{true};
  [[maybe_unused]] bool hasLooped{false};

  [[maybe_unused]] juce::Rectangle<int> bounds{};
  [[maybe_unused]] te::EditTimeRange    time{};
};


// State

using JuceValueTree [[maybe_unused]] = juce::ValueTree;
using JuceValue [[maybe_unused]] = juce::Value;

template<typename TObject>
using JuceCached [[maybe_unused]] = juce::CachedValue<TObject>;

using JuceVar [[maybe_unused]] = juce::var;


using JuceState [[maybe_unused]] = JuceValueTree;

using JuceStateListener [[maybe_unused]] = te::ValueTreeAllEventListener;
using JuceStateIdentifier [[maybe_unused]] = juce::Identifier;

template<typename TObject>
using JuceStateValue [[maybe_unused]] = juce::CachedValue<TObject>;

using JuceChangeBroadcaster [[maybe_unused]] = juce::ChangeBroadcaster;

using JuceChangeListener [[maybe_unused]] = juce::ChangeListener;


[[maybe_unused]] inline constexpr auto isJuceState =
    meta::traits::is_convertible_to ^ meta::type_c<juce::ValueTree&&>;

[[maybe_unused]] inline constexpr auto isJuceChangeBroadcaster =
    meta::traits::is_convertible_to ^ meta::type_c<juce::ChangeBroadcaster&&>;

// Selection

using JuceSelectionManager [[maybe_unused]] =
    te::SelectionManager;

using JuceSelectionManagerRef [[maybe_unused]] =
    te::SelectionManager*;

using JuceSelectionManagerConstRef [[maybe_unused]] =
    te::SelectionManager const*;


// Commands

using JuceCommandManager [[maybe_unused]] =
    juce::ApplicationCommandManager;

using JuceCommandId [[maybe_unused]] =
    juce::CommandID;

using JuceCommand [[maybe_unused]] =
    juce::ApplicationCommandTarget::InvocationInfo;

using JuceCommandInfo [[maybe_unused]] =
    juce::ApplicationCommandInfo;

using JuceCommandTarget [[maybe_unused]] =
    juce::ApplicationCommandTarget;


[[maybe_unused]] inline constexpr auto isJuceCommandTarget =
    meta::traits::is_convertible_to ^
    meta::type_c<juce::ApplicationCommandTarget&&>;


// Undo

using JuceUndoManager [[maybe_unused]] =
    juce::UndoManager;

using JuceUndoManagerRef [[maybe_unused]] =
    juce::UndoManager*;

using JuceUndoManagerConstRef [[maybe_unused]] =
    juce::UndoManager const*;


using JuceUndoableAction [[maybe_unused]] =
    juce::UndoableAction;


// Style

using JuceLookAndFeel [[maybe_unused]] = juce::LookAndFeel;

using JuceColourId [[maybe_unused]] = int;
using JuceColour [[maybe_unused]] = juce::Colour;

using JuceImage [[maybe_unused]] = juce::Image;
using JuceImageFileFormat [[maybe_unused]] = juce::ImageFileFormat;

using JuceGraphics [[maybe_unused]] = juce::Graphics;
using JuceDrawable [[maybe_unused]] = juce::Drawable;


[[maybe_unused]] inline constexpr auto isJuceLookAndFeel =
    meta::traits::is_convertible_to ^ meta::type_c<juce::LookAndFeel&&>;


// Context

using JuceEngine [[maybe_unused]] = te::Engine;

using JuceLogger [[maybe_unused]] = juce::Logger;


// Component

using JuceDesktop [[maybe_unused]] = juce::Desktop;

using JuceComponent [[maybe_unused]] = juce::Component;
using JuceConstrainer [[maybe_unused]] = juce::ComponentBoundsConstrainer;


using JuceToolbarItemId [[maybe_unused]] = int;


[[maybe_unused]] inline constexpr auto isJuceComponent =
    meta::traits::is_convertible_to ^ meta::type_c<juce::Component&&>;


// Window

using JuceWindow [[maybe_unused]] = juce::DocumentWindow;


[[maybe_unused]] inline constexpr auto isJuceWindow =
    meta::traits::is_convertible_to ^ meta::type_c<juce::TopLevelWindow&&>;

BLOOPER_STATIC_ASSERT(
    isJuceWindow(meta::type_c<juce::DocumentWindow>),
    "JuceWindow must satisfy JuceWindow.");


// Project

using JuceProject [[maybe_unused]] = te::Project;
using JuceProjectRef [[maybe_unused]] = JuceRef<te::Project>;
using JuceProjectConstRef [[maybe_unused]] = JuceConstRef<te::Project>;

using JuceProjectItem [[maybe_unused]] = te::ProjectItem;
using JuceProjectItemId [[maybe_unused]] = te::ProjectItemID;
using JuceProjectItemRef [[maybe_unused]] = JuceRef<JuceProjectItem>;
using JuceProjectItemConstRef [[maybe_unused]] = JuceConstRef<JuceProjectItem>;


// Edit

using JuceEdit [[maybe_unused]] = te::Edit;
using JuceEditRef [[maybe_unused]] = std::shared_ptr<JuceEdit>;
using JuceEditConstRef [[maybe_unused]] = std::shared_ptr<JuceEdit>;
using JuceEditWeakRef [[maybe_unused]] = std::weak_ptr<JuceEdit>;
using JuceEditWeakConstRef [[maybe_unused]] = std::weak_ptr<JuceEdit>;

using JuceTransport [[maybe_unused]] = te::TransportControl;
using JuceTransportRef [[maybe_unused]] = te::TransportControl*;
using JuceTransportConstRef [[maybe_unused]] = te::TransportControl*;

using JuceTempo [[maybe_unused]] = te::TempoSequence;
using JuceTempoRef [[maybe_unused]] = JuceTempo*;
using JuceTempoConstRef [[maybe_unused]] = JuceTempo*;

using JuceTempoSetting [[maybe_unused]] = te::TempoSetting;
using JuceTempoSettingRef [[maybe_unused]] = JuceRef<JuceTempoSetting>;
using JuceTempoSettingConstRef [[maybe_unused]] = JuceConstRef<JuceTempoSetting>;

using JuceTimeSigSetting [[maybe_unused]] = te::TimeSigSetting;
using JuceTimeSigSettingRef [[maybe_unused]] = JuceRef<JuceTimeSigSetting>;
using JuceTimeSigSettingConstRef [[maybe_unused]] = JuceConstRef<JuceTimeSigSetting>;


using JuceTrack [[maybe_unused]] = te::Track;
using JuceTrackRef [[maybe_unused]] = JuceRef<JuceTrack>;
using JuceTrackConstRef [[maybe_unused]] = JuceConstRef<JuceTrack>;

using JuceClipTrack [[maybe_unused]] = te::ClipTrack;
using JuceClipTrackRef [[maybe_unused]] = JuceRef<JuceClipTrack>;
using JuceClipTrackConstRef [[maybe_unused]] = JuceConstRef<JuceClipTrack>;

using JuceAudioTrack [[maybe_unused]] = te::AudioTrack;
using JuceAudioTrackRef [[maybe_unused]] = JuceRef<JuceAudioTrack>;
using JuceAudioTrackConstRef [[maybe_unused]] = JuceConstRef<JuceAudioTrack>;


[[maybe_unused]] inline constexpr auto isJuceTrack =
    meta::traits::is_convertible_to ^ meta::type_c<te::Track&&>;

[[maybe_unused]] inline constexpr auto isJuceTrackRef =
    meta::compose(
        meta::traits::is_convertible_to ^ meta::type_c<te::Track&>,
        meta::traits::dereferenced);

[[maybe_unused]] inline constexpr auto isJuceTrackConstRef =
    meta::compose(
        meta::traits::is_convertible_to ^ meta::type_c<const te::Track&>,
        meta::traits::dereferenced);

[[maybe_unused]] inline constexpr auto isJuceAudioTrack =
    meta::traits::is_convertible_to ^ meta::type_c<te::AudioTrack&&>;

[[maybe_unused]] inline constexpr auto isJuceAudioTrackRef =
    meta::compose(
        meta::traits::is_convertible_to ^ meta::type_c<te::AudioTrack&>,
        meta::traits::dereferenced);

[[maybe_unused]] inline constexpr auto isJuceAudioTrackConstRef =
    meta::compose(
        meta::traits::is_convertible_to ^ meta::type_c<const te::AudioTrack&>,
        meta::traits::dereferenced);

// Clip

using JuceClip [[maybe_unused]] =
    te::Clip;

using JuceClipRef [[maybe_unused]] =
    JuceRef<JuceClip>;

using JuceClipConstRef [[maybe_unused]] =
    JuceConstRef<JuceClip>;


using JuceAudioClip [[maybe_unused]] =
    te::AudioClipBase;

using JuceAudioClipRef [[maybe_unused]] =
    JuceRef<JuceAudioClip>;

using JuceAudioClipConstRef [[maybe_unused]] =
    JuceConstRef<JuceAudioClip>;


using JuceWaveAudioClip [[maybe_unused]] =
    te::WaveAudioClip;

using JuceWaveAudioClipRef [[maybe_unused]] =
    JuceRef<JuceWaveAudioClip>;

using JuceWaveAudioClipConstRef [[maybe_unused]] =
    JuceConstRef<JuceWaveAudioClip>;


using JuceMidiClip [[maybe_unused]] =
    te::MidiClip;

using JuceMidiClipRef [[maybe_unused]] =
    JuceRef<JuceMidiClip>;

using JuceMidiClipConstRef [[maybe_unused]] =
    JuceConstRef<JuceMidiClip>;


[[maybe_unused]] inline constexpr auto isJuceClip =
    meta::traits::is_convertible_to ^ meta::type_c<te::Clip&&>;

[[maybe_unused]] inline constexpr auto isJuceClipRef =
    meta::compose(
        meta::traits::is_convertible_to ^ meta::type_c<te::Clip&>,
        meta::traits::dereferenced);

[[maybe_unused]] inline constexpr auto isJuceClipConstRef =
    meta::compose(
        meta::traits::is_convertible_to ^ meta::type_c<const te::Clip&>,
        meta::traits::dereferenced);


[[maybe_unused]] inline constexpr auto isJuceAudioClip =
    meta::traits::is_convertible_to ^ meta::type_c<te::AudioClipBase&&>;

[[maybe_unused]] inline constexpr auto isJuceAudioClipRef =
    meta::compose(
        meta::traits::is_convertible_to ^ meta::type_c<te::AudioClipBase&>,
        meta::traits::dereferenced);

[[maybe_unused]] inline constexpr auto isJuceAudioClipConstRef =
    meta::compose(
        meta::traits::is_convertible_to ^ meta::type_c<const te::AudioClipBase&>,
        meta::traits::dereferenced);


// Takes

using JuceAudioTakeRef [[maybe_unused]] =
    te::ProjectItemID;

using JuceMidiTakeRef [[maybe_unused]] =
    te::MidiList*;


[[maybe_unused]] inline constexpr auto isJuceTakeRef =
    meta::satisfies_any(
        meta::traits::is_convertible_to ^ meta::type_c<te::ProjectItemID&&>,
        meta::traits::is_convertible_to ^ meta::type_c<te::MidiList*&&>);


// Thumbnails

using JuceThumbnail [[maybe_unused]] =
    te::SmartThumbnail;
using JuceThumbnailRef [[maybe_unused]] =
    JuceThumbnail*;
using JuceThumbnailConstRef [[maybe_unused]] =
    const JuceThumbnail*;

using JuceRecordingThumbnail [[maybe_unused]] =
    te::RecordingThumbnailManager::Thumbnail;
using JuceRecordingThumbnailRef [[maybe_unused]] =
    JuceRef<JuceRecordingThumbnail>;
using JuceRecordingThumbnailConstRef [[maybe_unused]] =
    JuceConstRef<JuceRecordingThumbnail>;


// Plugin

using JucePlugin [[maybe_unused]] =
    te::Plugin;

using JucePluginRef [[maybe_unused]] =
    JuceRef<JucePlugin>;

using JucePluginConstRef [[maybe_unused]] =
    JuceConstRef<JucePlugin>;

using JuceExternalPlugin [[maybe_unused]] =
    te::ExternalPlugin;

using JuceExternalPluginRef [[maybe_unused]] =
    JuceRef<JuceExternalPlugin>;

using JuceExternalPluginConstRef [[maybe_unused]] =
    JuceConstRef<JuceExternalPlugin>;


using JucePluginWindowState [[maybe_unused]] =
    te::PluginWindowState;


using JucePluginContent [[maybe_unused]] =
    juce::AudioProcessorEditor;

using JuceGenericPluginContent [[maybe_unused]] =
    juce::GenericAudioProcessorEditor;


[[maybe_unused]] inline constexpr auto isJucePlugin =
    meta::traits::is_convertible_to ^ meta::type_c<te::Plugin&&>;

[[maybe_unused]] inline constexpr auto isJucePluginRef =
    meta::compose(
        meta::traits::is_convertible_to ^ meta::type_c<te::Plugin&>,
        meta::traits::dereferenced);

[[maybe_unused]] inline constexpr auto isJucePluginConstRef =
    meta::compose(
        meta::traits::is_convertible_to ^ meta::type_c<const te::Plugin&>,
        meta::traits::dereferenced);


// Parameters

using JuceParameter [[maybe_unused]] =
    te::AutomatableParameter;

BLOOPER_NAMESPACE_END

#endif // BLOOPER_JUCE_TRAITS_HPP
