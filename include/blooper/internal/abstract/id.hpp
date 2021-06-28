#ifndef BLOOPER_ID_HPP
#define BLOOPER_ID_HPP
#pragma once

#include <blooper/internal/macros/macros.hpp>

BLOOPER_ID_NAMESPACE_BEGIN

// Old

BLOOPER_ID(editViewState);

BLOOPER_ID(showChordTrack);
BLOOPER_ID(showGlobalTrack);
BLOOPER_ID(showMarkerTrack);
BLOOPER_ID(showMidiDevices);
BLOOPER_ID(showWaveDevices);

BLOOPER_ID(drawWaveforms);
BLOOPER_ID(showHeaders);
BLOOPER_ID(showFooters);
BLOOPER_ID(showArranger);

BLOOPER_ID(viewX1);
BLOOPER_ID(viewX2);
BLOOPER_ID(viewY);


// Properties

BLOOPER_ID(properties);
BLOOPER_ID(property);
BLOOPER_ID(name);
BLOOPER_ID(value);


// State

BLOOPER_ID(windowState);

BLOOPER_ID(openProject);
BLOOPER_ID(monitored);


// Settings

BLOOPER_ID(appearance);
BLOOPER_ID(behaviour);
BLOOPER_ID(devices);
BLOOPER_ID(plugins);
BLOOPER_ID(projects);

// Appearance

BLOOPER_ID(theme);
BLOOPER_ID(themePath);

BLOOPER_ID(trackSize);
BLOOPER_ID(trackClipsSize);

BLOOPER_ID(masterTrackPanelSize);
BLOOPER_ID(controlSurfacePanelSize);
BLOOPER_ID(browserPanelSize);

// Behaviour

BLOOPER_ID(rootBrowserFolder);

// Plugins

BLOOPER_ID(isPluginDPIAware);


// Project Settings

BLOOPER_ID(bpm);

// Project State

BLOOPER_ID(edit);

BLOOPER_ID(track);

BLOOPER_ID(mode);
BLOOPER_ID(interval);
BLOOPER_ID(playback);

BLOOPER_ID(soloed);
BLOOPER_ID(muted);
BLOOPER_ID(armed);

BLOOPER_ID_NAMESPACE_END

#endif // BLOOPER_ID_HPP
