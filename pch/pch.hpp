#ifndef BLOOPER_PCH
#define BLOOPER_PCH


// push warning suppression

#if defined(__clang__) // clang

    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunused-parameter"

#elif defined(_MSC_VER) // MSVC

    #pragma warning(push)
    #pragma warning(disable : 4100) // unreferenced formal parameter
    #pragma warning(disable : 4996) // used deprecated declaration

    #pragma warning(disable : 6297) // arithmetic overflow

#elif defined(__GNUC__) // GNU

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-arguments"

#else // compiler

    #error "Unsupported C++ compiler."

#endif // compiler


// env

#include <env/env.hpp>
#include <env/macros.hpp>

#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>

#include <juce_data_structures/juce_data_structures.h>


// audio

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_plugin_client/juce_audio_plugin_client.h>
#include <juce_dsp/juce_dsp.h>

#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_audio_utils/juce_audio_utils.h>

#include <tracktion_graph/tracktion_graph.h>


// ui

#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>

#include <tracktion_engine/tracktion_engine.h>


// pop warning suppression

#if defined(__clang__) // clang

    #pragma clang diagnostic pop

#elif defined(_MSC_VER) // MSVC

    #pragma warning(pop)

#elif defined(__GNUC__) // GNU

    #pragma GCC diagnostic pop

#else // compiler

    #error "Unsupported C++ compiler."

#endif // compiler


#endif // BLOOPER_PCH
