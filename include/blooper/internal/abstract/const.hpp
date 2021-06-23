#ifndef BLOOPER_CONST_HPP
#define BLOOPER_CONST_HPP
#pragma once

#include <blooper/internal/macros/namespaces.hpp>

BLOOPER_NAMESPACE_BEGIN

// callbacks

inline constexpr auto delayedCallbackDelayMilliseconds{50};


// time

inline constexpr auto defaultBpm{120};


// context

inline constexpr static auto contextKey = "context";

inline constexpr static auto rootDirSpecialLocation =
    JuceFile::SpecialLocationType::userHomeDirectory;

inline constexpr static auto rootDirName{".blooper"};


inline constexpr static auto engineSettingsFileName{"engineSettings.xml"};
inline constexpr static auto settingsFileName{"settings.xml"};
inline constexpr static auto stateFileName{"state.xml"};

inline constexpr static auto logDirName{"log"};
inline constexpr static auto logFileNamePrefix{"blooper_"};
inline constexpr static auto logFileNameSuffix{".log"};

inline constexpr static auto projectsDirName{"projects"};


// properties

inline constexpr auto cacheDirName{".cache"};
inline constexpr auto prefsDirName{"prefs"};

inline constexpr auto mediaDirName{"media"};

inline constexpr auto propertiesKey{"properties"};


// app

inline constexpr auto appName{JUCE_APPLICATION_NAME_STRING};
inline constexpr auto appVersion{JUCE_APPLICATION_VERSION_STRING};


// plugins

#if JUCE_LINUX
inline constexpr bool shouldAddPluginWindowToDesktop{false};
#else
inline constexpr bool shouldAddPluginWindowToDesktop{true};
#endif

inline constexpr auto pluginIdSuffix{"_plugin"};


// gui

inline constexpr auto lineSpace{6};


// colour

inline constexpr JuceColourId minimalBlooperColourId{100000000};

BLOOPER_NAMESPACE_END

#endif // BLOOPER_CONST_HPP
