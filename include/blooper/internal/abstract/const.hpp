#ifndef BLOOPER_CONST_HPP
#define BLOOPER_CONST_HPP
#pragma once

#include <blooper/internal/macros/namespaces.hpp>

BLOOPER_NAMESPACE_BEGIN

#ifdef __JETBRAINS_IDE__
  #pragma clang diagnostic push
  #pragma ide diagnostic   ignored "cert-err58-cpp"
#endif


// callbacks

inline constexpr auto delayedCallbackDelayMilliseconds{50};


// time

inline constexpr auto defaultBpm{120};


// context

inline constexpr auto contextKey{"context"};

inline constexpr auto rootDirSpecialLocation{
    juce::File::SpecialLocationType::userHomeDirectory};

inline constexpr auto rootDirName{".blooper"};


inline constexpr auto engineSettingsFileName{"engineSettings.xml"};
inline constexpr auto settingsFileName{"settings.xml"};
inline constexpr auto stateFileName{"state.xml"};

inline constexpr auto logDirName{"log"};
inline constexpr auto logFileNamePrefix{"blooper_"};
inline constexpr auto logFileNameSuffix{".log"};

inline constexpr auto projectsDirName{"projects"};


// properties

inline constexpr auto cacheDirName{".cache"};
inline constexpr auto prefsDirName{"prefs"};

inline constexpr auto mediaDirName{"media"};

inline constexpr auto propertiesKey{"properties"};


// app

inline constexpr auto appName{JUCE_APPLICATION_NAME_STRING};
inline constexpr auto appVersion{JUCE_APPLICATION_VERSION_STRING};

inline constexpr auto quitRetryIntervalMilliseconds{100};


// plugins

#if JUCE_LINUX
inline constexpr bool shouldAddPluginWindowToDesktop{false};
#else
inline constexpr bool shouldAddPluginWindowToDesktop{true};
#endif

// closest to 100 width
inline constexpr auto minimumPluginEditorWidth{128};
inline constexpr auto minimumPluginEditorHeight{72};

// 8K
inline constexpr auto maximumPluginEditorWidth{7680};
inline constexpr auto maximumPluginEditorHeight{4320};

inline constexpr auto builtinPluginFormatName{"Internal"};

inline constexpr auto builtinUniqueIdSuffix{"_internal"};
inline constexpr auto effectPluginUniqueIdSuffix{"_effect"};
inline constexpr auto synthPluginUniqueIdSuffix{"_synth"};
inline constexpr auto pluginUniqueIdSuffix{"_plugin"};
inline constexpr auto pluginIdSuffix{"_plugin"};
inline constexpr auto rackUniqueIdSuffix{"_rack"};

inline const auto builtinsFolderName{TRANS("Builtins")};
inline const auto racksFolderName{TRANS("Racks")};

inline constexpr auto builtinEffects{
    env::meta::make_tuple(
        env::meta::type_c<te::VolumeAndPanPlugin>,
        env::meta::type_c<te::EqualiserPlugin>,
        env::meta::type_c<te::ReverbPlugin>,
        env::meta::type_c<te::DelayPlugin>,
        env::meta::type_c<te::ChorusPlugin>,
        env::meta::type_c<te::PhaserPlugin>,
        env::meta::type_c<te::CompressorPlugin>,
        env::meta::type_c<te::PitchShiftPlugin>,
        env::meta::type_c<te::LowPassPlugin>,
        env::meta::type_c<te::MidiModifierPlugin>,
        env::meta::type_c<te::MidiPatchBayPlugin>,
        env::meta::type_c<te::PatchBayPlugin>,
        env::meta::type_c<te::AuxSendPlugin>,
        env::meta::type_c<te::AuxReturnPlugin>,
        env::meta::type_c<te::TextPlugin>,
        env::meta::type_c<te::FreezePointPlugin>,
        env::meta::type_c<te::InsertPlugin>)};

inline constexpr auto builtinSynths{
    env::meta::make_tuple(
        env::meta::type_c<te::SamplerPlugin>,
        env::meta::type_c<te::FourOscPlugin>)};


// gui

inline constexpr auto pathSeparator = "/";

inline constexpr auto defaultGuiTimerHz{20};

inline constexpr auto outlineThickness{2};
inline constexpr auto padding{2};
inline constexpr auto outlinePaddingFactor{3};

inline constexpr auto defaultTrackSize{100};

inline constexpr auto defaultMasterTrackPanelSize{100};
inline constexpr auto defaultControlSurfacePanelSize{100};
inline constexpr auto defaultBrowserPanelSize{100};

inline constexpr auto defaultMasterTrackPanelOpen{true};
inline constexpr auto defaultControlSurfacePanelOpen{false};
inline constexpr auto defaultBrowserPanelOpen{false};


// colour

inline constexpr auto minimalBlooperColourId{100000000};


#ifdef __JETBRAINS_IDE__
  #pragma clang diagnostic pop
#endif

BLOOPER_NAMESPACE_END

#endif // BLOOPER_CONST_HPP
